/**
  ******************************************************************************
  * @file    main.c  
  * @author  INSA
  * @version V1.2 Mis en propre
  * @date    13-Juin-2017
  * @brief    
  ******************************************************************************
  * @attention
  *Lignes utilisées sur connecteurs Discovery hors alimentations :
	*ADC1 : 1.2 Khz environ le cyscle complet d'un balayage de toutes les entrées
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
  * Informations Moteurs :
	* MY1020 36V 18.3A MAX 500W 2500RPM 0,47 mH 0.5 ohm
	*TODO :
	* Cal gyro Fait avec zero figé accelero et immobilité départ
	* IT priorité sytic a redescendre ou suppression avec lcd
	*Delta t gyro 1/95 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ******************************************************************************
  */
	/* Les calibrations : */
	/****************************************************************************/ 
  /* Le gyropode doit rester immobile                                         */	
	/*                                                                          */
  /* Pour recaler l'angle aller à :                                           */ 
  /* ZONE DE CAL ACCELERO                                                     */	
	/*                                                                          */
  /* Pour recaler mesure I moteur :  mesurer atténuation COEFI                */ 
  /* Sortie capteur courant atténuée V = (1/COEFI) X [ 2.5 +0.625/6  X Im]    */
  /* Im1 = V X COEFI X 9.6 - 24  : COEFI1 = 1.6913                       */

  /* COEFI1(2) : Mesurer atténuation pont diviseur en sortie du capteur       */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

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


/**
  * @brief  Main program.
  * @param  None 
  * @retval None
  */
int main(void)
{  

  uint8_t i = 0;
	/* conf lignes PortC, commande alim général et arrêt moteurs	*/
	Init_shutdown();// conf lignes PortC, commande alim général et arrêt moteurs
	
  /* Retourne fréquence systeme */
	RCC_GetClocksFreq(&RCC_Clocks);
	
	/* configuration du systick Tps = 1 / nbre*/
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100000);
	NVIC_SetPriority(SysTick_IRQn, 3);// priorité faible pour le Systick
	/* Initialisation LEDs usage général pour test (et User Button)   STM32F3-Discovery board */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
  STM_EVAL_LEDInit(LED7);
  STM_EVAL_LEDInit(LED8);
  STM_EVAL_LEDInit(LED9);
  STM_EVAL_LEDInit(LED10);
  //STM_EVAL_LEDOn(LED3);	

	//  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI); 
	// 	NVIC_SetPriority(EXTI0_IRQn, 0);// priorité 0 pour l'IT du bouton
	
	/****************************************************************************/
	/* Initialisation  afficheur LCD                                            */	
	/*Configuration PORTD PD0...PD6 en sortie push-pull de l'afficheur LCD      */
	/****************************************************************************/

	/* GPIODPeriph clock enable */
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOD , ENABLE);
	/*Lignes OUT du PortD*/
	GPIO_InitStructure.GPIO_Pin =  LCD_LIGNES_UTILES ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/*Initialisation ADC1 et DMA1*/

	/* Configuration ADC1 clock */
  RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);// ADC1 et ADC2 de Div1 à Div256
  /* Enable ADC1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);//ADC1 et ADC2 clock en commun
      
  Init_ADC1_DMA1();
  Init_IT_DMA1();
  NVIC_SetPriority(DMA1_Channel1_IRQn, 1);// priorite 1
	/*Initialisation USART2*/
  Delay(50000);//0.5s
	Init_USART();
	/*Initialisation TIMER15 & 1*/
  TIM_Config();
	//initialisation TIMER15 pour PWM
  /* Reset UserButton_Pressed variable */
	//UserButtonPressed = 0x00; 
   
  /* Infinite loop */
  while (1)
  {   
  /*Attente appui bouton user carte Discovery*/  
	//     while (UserButtonPressed == 0x00)
	//     {
	//     }
    Delay(50000);//0.5s
		
    USARTstring(TX_USART);// Envoi initial chaine passer par la DMA ensuite obligatoirement		
    Delay(200000);//2s
    STM_EVAL_LEDOn(LED5);	

   
  /* Initialisation LSM303DLHC  Accelerometres            */
		
	  Acc_config();

  /* Initialisation Gyroscope L3GD20  L3GD20_FULLSCALE_500*/

    Gyro_config();
	  Delay(100000);//1s
		
  /* Zero accX, compensation avant le gyro important*/	
		
		Acc_read(AccBuffer);
		
		
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
		
  /* Initialisation de ligne INT2 en IT du gyro sur PE1 */
	// Delay(100000);//1s
		Init_IT_EXT();
		NVIC_SetPriority(EXTI1_IRQn, 2);// priorité 2 INT2 en IT du gyro sur PE1
		Anglegyro =0;
	/************************/ 
	/* ZONE DE CAL LT6      */	
	/************************/
	/* V(in ADC) =(1/COEF)  X { V0 +(.625/6) X IM}  formule brute
   - V0 sortie capteur à vide , voisine de 2.5 V soit V01 ou V02 pour moteur 1 & 2
	 - COEF : attenuation du pont diviseur de resitances ( en sortie LTS6 et entree ADC) soit COEF1&2 por moteur 1 & 2
	 
	 on pose VIM10 ou VIM20 = V01(2) * 1/COEF1(2) tension mesurée in ADC, à vide
   - V = VIM10(20)+ 1/COEF1(2) X 0.625/6 X IM1(2)) en sortie pont diviseur et entrée ADC
	 Moteur 1 ou 2 :	
	 Les tensions mesurées sont en mV de 0 à 3000 mv, dynamique ADC choisi
	 IM1&2 =(V-VIM10)10^-3 X 9.6 X 100/ COEF1(2) donne format XX.XX  virgule ( X 1/100)
			=0.96 (V-V1M10)X COEF1(2)
	*/ 
	/* Mesure tension à vide sur entée ADC, PONT en H off*/
		ADC1ConvertedValue =  MOY_ADC12_IN6; //moteur 1
		VIM10 =  (ADC1ConvertedValue *ScaleADC/0xFFF);//  resultat en mv 3000 max
		ADC1ConvertedValue =  MOY_ADC12_IN7; //moteur 2
		VIM20 =  (ADC1ConvertedValue *ScaleADC/0xFFF);// resultat en mv 3000 max

	/* Lecture niveau   priorite d'une IT sur NVIC  : */		
	// toto =  NVIC_GetPriority( EXTI1_IRQn );
  /* Waiting User Button is pressed */
	
		while (1)
		{
			/* Zone Attente IT */
		}
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

/************************ (C) COPYRIGHT INSA TOULOUSE ************************/
