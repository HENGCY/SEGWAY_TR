/**
  ******************************************************************************
  * @file    main.c  
  * @author  INSA Toulouse
  * @version V2.0 
  * @date    23 Juin 2017 
  * @brief    
  *******************************************************************************/
	
 /* Les informations :
  *Lignes utilisées sur connecteurs Discovery hors alimentations :
	*PC0 ADC12_IN6 courant moteur 1 associé à coefI1
 	*PC1 ADC12_IN7 courant moteur 2 associé à coefI2
	*PC2 ADC12_IN8 Direction
	*PC3 ADC12_IN9 tension de batterie
  *PA1 ADC1_IN2	   
  *PF4 ADC1_IN5	 
	*PF2 ADC_IN10 option non codé
	*PA0 ADC_IN10 option non codé Desouder SB20 nn fait
  * 
  *USART2 :	
  *PA3 USART2 TX
  *PA2 USART2 RX  
  *  
	*PWM1 Moteur 1
  *PA8 est TIM1_CH1  AF6
  *PA11 est TIM1_CH1N AF6    SB22 à souder carte Dicovery   
  *  
 	*PWM2 Moteur 2
  *PA9 est TIM1_CH2 AF6
  *PA12 est TIM1_CH2N AF6    SB21 à souder carte Dicovery  
	*
  *SHUTDOWN MOTEUR 1 & 2 actif 0
	*PC6 Shutdown Moteur 1 
	*PC7 Shutdown Moteur 2 
	*
  *SHUTDOWN GENERAL COMMANDE ALIM actif 1
  *PC8 CMD_ALIM
	*
  *PWM  TIMER 15 NON CONNECTE:  
  *PB14 est TIM5_CH1  AF1
  *PB15 est TIM5_CH1N AF2  
	*
	*Boutons poussoirs du guidon 
	*PA4 A coder
  *Informations Moteurs :
	*MY1020 36V 18.3A MAX 500W 2500RPM 0,47 mH 0.5 ohm

  ******************************************************************************
  */
	
	
 /* Les calibrations : 
   * Le gyropode doit rester immobile                                       
	 *                                                                          
   * Pour recaler l'angle aller à :                                           
   * ZONE DE CAL ACCELERO                                                     
	 *                                                                         
   * Pour recaler mesure I moteur :  mesurer atténuation COEFI                
   * Sortie capteur courant atténuée V = (1/COEFI) X [ 2.5 +0.625/6  X Im]   
   * Im1 = V X COEFI X 9.6 - 24  : COEFI1 = 1.6913                       
	 * COEFI1(2) : Mesurer atténuation pont diviseur en sortie du capteur       */ 


/*Status de fonctionnement ST:
00: ras
01: low bat
02: arret urgent demandé
......
09: ...
10 : Probleme capteur0
11 :   "      capteur1   
......
20 : CAL GYRO démarrage
......
99 : ...
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Variable global------------------------------------------------------------*/
bool ConfigOK = false;			// Indicataion de fin de configuration des peripherique et interruptions (tilisé en boucle de courant)

/**
  * @brief  Main program.
  * @param  None 
  * @retval None
  */
int main(void)
{  
  ConfigOK=false;																				// 	Configuration du système n'est pas fini						
	
	// Initialisation LEDs de STM32F3-Discovery board 
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
  STM_EVAL_LEDInit(LED7);
  STM_EVAL_LEDInit(LED8);
  STM_EVAL_LEDInit(LED9);
  STM_EVAL_LEDInit(LED10);
	
	Init_shutdown();																			// 	Configuration PortC, commande alim général et arrêt moteurs
	RCC_GetClocksFreq(&RCC_Clocks);  											// 	Retourne la fréquence systeme 
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100000);		// 	Configuration du systick Tps = 1 / nbre
	NVIC_SetPriority(SysTick_IRQn, 3);										// 	Configuration priorité faible pour le Systick

	// Initialisation et configuration de ADC-DMA
  RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);								// 	ADC1 et ADC2 de Div1 à Div256
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);		//	ADC1 et ADC2 clock en commun
  Init_ADC1_DMA1();																			//	Configuration ADC1 
  Init_IT_DMA1();																				//	Configuration d'interruption de DMA pour ADC1 
  NVIC_SetPriority(DMA1_Channel1_IRQn, 1);							//  Configuration priorité 1 pour DMA
	
	STM_EVAL_LEDOn(LED3);																	// 	LED indication de étape fait 
  Delay(50000);//0.5s
	
	Init_USART();																					//	Initialisation USART2
  TIM_Config();																					//	Initialisation des Timers (PWMs de moteurs)
	Init_IT_TIM2();																				// 	Activation interruption timer 2
	NVIC_SetPriority(TIM2_IRQn, 0);												//  Configuration priorité 0 pour TIM 2
	
 
	// Mesure tension à vide sur entrée ADC (offset pour calcul de courant), PONT en H off
	ADC1ConvertedValue =  MOY_ADC12_IN6; 									//	Tension à vide moteur 1 en echelle ADC
	VIM10 =  (ADC1ConvertedValue *ScaleADC/0xFFF);				//  Resultat en mv 3000 max
	ADC1ConvertedValue =  MOY_ADC12_IN7; 									//	Tension à vide moteur 2 en echelle ADC
	VIM20 =  (ADC1ConvertedValue *ScaleADC/0xFFF);				//  Resultat en mv 3000 max
		
  STM_EVAL_LEDOn(LED5);																	// 	LED indication de étape fait 
 
	Acc_config(); 																				//	Initialisation LSM303DLHC  Accelerometr
  Gyro_config();																				// 	Initialisation Gyroscope L3GD20 par default avec L3GD20_FULLSCALE_500
	Delay(100000);																				// 	Delay de 1s
	Acc_read(AccBuffer);  																//  Valeur initial(Zero) de accX, compensation avant le gyro important
			
	/************************/ 
	/* ZONE DE CAL ACCELERO */	
	/************************/

	// ZeroACCX = -AccBuffer[0];// A activer : zero accelero carte à plat a lire en debug et à  mémoriser en ligne suivante
	// mettre un poit d'arret 3 lignes plus bas pour premier passage
 		ZeroACCX = -37;// cal mémorisé sur le zero accelero, le max est moins utile 
  /* Mise à 0 de la ligne INT2 de L3GD20 sur in PE1	par lecture d'une  mesure*/
		Gyro_read(Buffer);
		STM_EVAL_LEDOn(LED7);	
  /* Zero du gyro, compensation*/	
		Zerogyro = -Buffer[0];// A faire en arret en cal ou moyenner TODO ??????????????????????????????????????????????	
 
 
	Init_IT_EXT();																			//	Configuration d'interruption de EXTI pour gyro-acc à 94 Hz par default
	NVIC_SetPriority(EXTI1_IRQn, 2);										// 	Priorité 2  en IT du gyro
	Anglegyro =0;

	ConfigOK=true;																			// 	Configuration du système est fini						
	while (1)										
	{
			/* Zone Attente IT */
	}
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif
