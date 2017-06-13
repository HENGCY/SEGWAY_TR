/**
  ******************************************************************************
  * @file    main.c  
  * @author  INSA
  * @version V1.1.0 Fusion de donnees (Gyro et Accelero) - USART - ADC
  * @date    Avil- mai 2017 
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
  *
	*PWM1 Moteur 1
  *PA8 est TIM1_CH1  AF6
  *PA11 est TIM1_CH1N AF6    SB22 à souder carte Dicovery   
	
  *  
 	*PWM2 Moteur 2
  *PA9 est TIM1_CH2 AF6
  *PA12 est TIM1_CH2N AF6    SB21 à souder carte Dicovery  
	
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
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x

#define L3G_Sensitivity_250dps     (float)   114.285f         /*!< gyroscope sensitivity with 250 dps full scale [LSB/dps] */
#define L3G_Sensitivity_500dps     (float)    57.1429f        /*!< gyroscope sensitivity with 500 dps full scale [LSB/dps] */
#define L3G_Sensitivity_2000dps    (float)    14.285f	      /*!< gyroscope sensitivity with 2000 dps full scale [LSB/dps] */

/* Mettre 250 avec 250 ou 500 avec 500  ou 2000 avec 2000 :*/
#define L3G_Sensitivity L3G_Sensitivity_500dps 
#define L3GD20_SCALE     L3GD20_FULLSCALE_500 // L3GD20_FULLSCALE_250 L3GD20_FULLSCALE_500   L3GD20_FULLSCALE_2000 

#define PI                         (float)     3.14159265f 

#define LSM_Acc_Sensitivity_2g     (float)     1.0f            /*!< accelerometer sensitivity with 2 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_4g     (float)     0.5f            /*!< accelerometer sensitivity with 4 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_8g     (float)     0.25f           /*!< accelerometer sensitivity with 8 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_16g    (float)     0.0834f         /*!< accelerometer sensitivity with 12 g full scale [LSB/mg] */
#define Deltatps                   (float)     0.010526f       /*!< Delta t du Gyro  = 1/95 HZ */
#define ScaleADC                    3000                       /*pleine echelle ADC 3V */

#define facteur_prescaler 1 // facteur pour TIMER 15
#define Freq_PWM 25000// frequence PWM en Hz  
#define Freq_SYS 72000000// frequence systeme
#define PWM50  (Freq_SYS/(2*Freq_PWM))// initielisation compteur Timer pour 50 %
#define SHDNM1 GPIO_Pin_6 //shutdown moteur 1 PC6
#define SHDNM2 GPIO_Pin_7 //shutdown moteur 1 PC7
#define CMD_ALIM GPIO_Pin_8//Commande alim générale
#define COEFI1  (float) 1.71283f // attenuateur capteur courant moteur 1  = 2.523/1.473 sortie capteur/ entrée mesure à vide in ADC
#define COEFI2  (float) 1.71584f // attenuateur capteur courant moteur 2  = 2.512/1.464 sortie capteur/ entrée mesure à vide in ADC

/* Private variables ---------------------------------------------------------*/
/*RCC_ClocksTypeDef RCC_Clocks;
GPIO_InitTypeDef  GPIO_InitStructure;	
USART_InitTypeDef USART_InitStructure;
ADC_InitTypeDef       ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
DMA_InitTypeDef DMA_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;*/

/* Déclarations chaines caracteres a transmettre */	
uint8_t lin [17]    = "  ADC:         V";// ADC test RS232C
uint8_t lin1 [17]   = "Fusio:       deg";
uint8_t TX_USART [46]   = "M0:+0000M1:+0000M2:+0000M3:+0000M4:+0000ST:20\r"; //Buffer emission
uint8_t RX_USART [10]   = ""; //Buffer reception
uint8_t ST[2]   = "XX"; //test RX serie

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

/*ADC1 DMA1 variables */
__IO uint32_t  ADC1ConvertedValue = 0, ADC1ConvertedVoltage = 0;
__IO uint16_t Acq_ADC1[6], calibration_value = 0;
__IO uint16_t MOY_ADC12_IN6 = 0, MOY_ADC12_IN7 = 0, MOY_ADC12_IN8 = 0, MOY_ADC12_IN9 =0,MOY_ADC1_IN2 =0,MOY_ADC1_IN5 =0;;// variables lectures ADC moyennees de Acq_ADC1[4]
__IO int32_t  IM1,IM2,VIM10,VIM20;
__IO uint32_t TimingDelay = 0, toto;
__IO uint32_t UserButtonPressed = 0;
// __IO float HeadingValue = 0.0f;  

float MagBuffer[3] = {0.0f}, AccBuffer[3] = {0.0f}, Buffer[3] = {0.0f};
float Anglegyro = 0.0f;// calcule de l'angle a partir de d °/seconde
float Anglemesure = 0.0f;// calcule de l'angle fusionné
float VitesseAngle = 0.0f;// vitesse angulaire

float Zerogyro = 0.0f;// zero du gyro
float ZeroACCX = 0.0f;// zero accelero X

float Angleacc = 0.0f;// calcule de l'angle a partir de tangente inverse
uint16_t Xval, Yval, Zval = 0x00;
uint16_t Angleval = 0x00;

__IO uint8_t DataReady = 0;
__IO uint8_t PrevXferComplete = 1;
__IO uint32_t USBConnectTimeOut = 100;
//TIMER ET PWM
uint16_t TimerPeriod = 0;
uint16_t PWM_MOTEUR1 = PWM50 ;// 50% PWM Moteur PA8  PA11
uint16_t PWM_MOTEUR2 = PWM50 ;// 50% PWM Moteur PA9  PA12
//Variables calcul
uint16_t Tampon1 = 0 ; 

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None 
  * @retval None
  */
int main(void)
{  
  uint8_t i = 0;
	/* conf lignes PortC, commande alim général et arrêt moteurs	*/
	ShutdownInit();// conf lignes PortC, commande alim général et arrêt moteurs
	
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
  STM_EVAL_LEDOn(LED3);	

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
  IT_DMA1_Init();
  NVIC_SetPriority(DMA1_Channel1_IRQn, 1);// priorite 1
	/*Initialisation USART2*/
  Delay(50000);//0.5s
	USARTINIT();
	/*Initialisation TIMER15 & 1*/
  TIM_Config();//initialisation TIMER15 pour PWM
  /* Reset UserButton_Pressed variable */
	//   UserButtonPressed = 0x00; 
   
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
		
	  Demo_CompassConfig();

  /* Initialisation Gyroscope L3GD20  L3GD20_FULLSCALE_500*/

    Demo_GyroConfig();
	  Delay(100000);//1s
		
  /* Zero accX, compensation avant le gyro important*/	
		
		Demo_CompassReadAcc(AccBuffer);
		
		
	/************************/ 
	/* ZONE DE CAL ACCELERO */	
	/************************/

	// ZeroACCX = -AccBuffer[0];// A activer : zero accelero carte à plat a lire en debug et à  mémoriser en ligne suivante
	// mettre un poit d'arret 3 lignes plus bas pour premier passage
 		ZeroACCX = -37;// cal mémorisé sur le zero accelero, le max est moins utile 
  /* Mise à 0 de la ligne INT2 de L3GD20 sur in PE1	par lecture d'une  mesure*/	

		Demo_GyroReadAngRate(Buffer);
		STM_EVAL_LEDOn(LED7);	
  /* Zero du gyro, compensation*/	

		Zerogyro = -Buffer[0];// A faire en arret en cal ou moyenner TODO ??????????????????????????????????????????????
		
  /* Initialisation de ligne INT2 en IT du gyro sur PE1 */
	// Delay(100000);//1s
		IT_EXT_Init();
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

/**
  * @brief  Configure the L3GD2 to gyroscope application.
  * @param  Sensibilité ±250/±500/ ±2000 dps SPI 
  * @param  Digital output data rate (ODR) 95/190/380/760
  */
 

/*Initialisations des lignes shutdown */
/*
  *SHUTDOWN MOTEUR 1 & 2 actif 0
	*PC6 Shutdown Moteur 1 :  SHDNM1
	*PC7 Shutdown Moteur 2 :  SHDNM2
	*
  *SHUTDOWN GENERAL COMMANDE ALIM actif 0
  *PC8 Activation alim générale : CMD_ALIM
	*
*/

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Traitement donnee Gyro, Accelero et ADC
  * @param   
  * @retval  
  */

GPIO_SetBits(GPIOC,SHDNM1 | SHDNM2);// moteurs 1&2 ON


if (((ST[0]<58) && (ST[0]>47))&&((ST[1]<58) && (ST[1]>47)))// test 00 à 99 ASCII
{
	Tampon1 =(2* PWM50*( (ST[0]-48)*10 +  ST[1]-48))/100;// conversion en %
	PWM_MOTEUR1 = Tampon1;
	PWM_MOTEUR2 = Tampon1;

	TIM_SetCompare1( TIM1,  PWM_MOTEUR1);// ecriture de CCR1 de 16 bits
	TIM_SetCompare2( TIM1,  PWM_MOTEUR2);// ecriture de CCR2 de 16 bits
		
}

//***********************************
			
      STM_EVAL_LEDOff(LED10);//Test fin de calcul
			
//***********************************
}


/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t LSM303DLHC_TIMEOUT_UserCallback(void)
{
  return 0;
}

/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t L3GD20_TIMEOUT_UserCallback(void)
{
  return 0;
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
