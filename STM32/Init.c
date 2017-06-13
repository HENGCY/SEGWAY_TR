/**
  ******************************************************************************
  * @file    Init.c
  * @author  INSA Toulouse
  * @version V1.0
  * @date    13-Juin-2017
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Init.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

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
void	Init_shutdown(void)
{
	/* GPIODPeriph clock enable */
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOC , ENABLE);
	/*Lignes OUT du PortD*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	//  GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
	//  GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
	GPIO_SetBits(GPIOC,CMD_ALIM);// commande alimentation général '1'
	GPIO_ResetBits(GPIOC,SHDNM1 | SHDNM2);// Arret des moteurs par '0'	
}

void Init_IT_EXT(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  
  /* Enable GPIOE clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  
  /* Configure PE1 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Connect EXTI1 Line to PE1 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);

  /* Configure EXTI1 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;// ligne 1 bit 1 PE1 INT2 accelero
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//EXTI0_IRQn
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

void Init_IT_DMA1(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);//activation
}

void Init_ADC1_DMA1(void)
{	
  /* ADC Channel configuration */
  /* GPIOC &  GPIOA Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);//clock port C
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);// clock port A
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);// clock port F

  /* Configure ADC1 : Channel 6,7,8,9 Port C & chanel 2 PortA & channel 5 PortF*/
	/* PC0 ADC12_IN6 PC1 ADC12_IN7 PC2 ADC12_IN8 PC3 ADC12_IN9 Pin communes aux deux  ADC1&2*/
  /* PA1 est ADC1_IN2, PF4 est ADC1_IN5 */
	/* lignes communes  a ADC1 et ADC2 "Additional functions"*/
	//PortC 6,7,8,9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3;//GPIO_Pin PortC
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//PortA PA1
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1; //GPIO_Pin_1 PortA
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PortF PF4
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4; //GPIO_Pin_4 PortF
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	/*configuration DMA1 associé à ADC1 Cycle complet 5 mesures > 700Hz*/
	/* Enable DMA1clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);// adresse du registre data sortie ADC1
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Acq_ADC1;// adresse de destinacion dans la mémoire 
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;// le peripherique est la source vers la mémoire
  DMA_InitStructure.DMA_BufferSize = 6;// taille du buffer pour IN6 à IN9,  IN1 et IN5 6 mesures
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//incrementation registre peripherique non
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//incrementation adresse memoire oui
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//16 bits
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;// 16 bits
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// circulaire
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// memoire a memoire non

  DMA_Init(DMA1_Channel1, &DMA_InitStructure);// chanel1 DMA1 dispo pour ADC1  
	
  /* Calibration procedure */  
  ADC_StructInit(&ADC_InitStructure);//defaut

  ADC_VoltageRegulatorCmd(ADC1, ENABLE);
  
  /* Insert delay equal to 10 µs */
  Delay(10);// mettre 1 !!!!
  
  ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC1);
  
  while(ADC_GetCalibrationStatus(ADC1) != RESET );
  calibration_value = ADC_GetCalibrationValue(ADC1);
	
  /* Configure ADC1 : Channel 6,7,8,9 Port C & chanel 2 PortA & channel 5 PortF*/
	/* PC0 ADC12_IN6 PC1 ADC12_IN7 PC2 ADC12_IN8 PC3 ADC12_IN9 Pin communes aux deux  ADC1&2*/
  /* PA1 est ADC1_IN2, PF4 est ADC1_IN5 */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent  ;//  ADC_Mode_Independent                                                           
  ADC_CommonInitStructure.ADC_Clock =ADC_Clock_AsynClkMode   ;// ou ADC_Clock_SynClkModeDiv1,2...                    
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled  ;  //ADC1_CCR    MDMA[1:0] bit 14 & 15   for dual mode Em mode simpe -->  off   
  ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_Circular;// circulaire                  
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;          
  ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
  
  ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;//
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;// 
  ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;         
  ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;//
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//
  ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;   
  ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;  
  ADC_InitStructure.ADC_NbrOfRegChannel = 6;// 6 voies
  ADC_Init(ADC1, &ADC_InitStructure);
  
  /* ADC1 regular channel 6,7,8,9,1 & 5 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_19Cycles5 );//PC0 ADC12_IN6
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 2, ADC_SampleTime_19Cycles5 );//PC1 ADC12_IN7
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_19Cycles5);//PC2 ADC12_IN8 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_19Cycles5 );//PC3 ADC12_IN9
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 5, ADC_SampleTime_19Cycles5 );//PA1 ADC1_IN2
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_19Cycles5 );//PF4 ADC1_IN5

	
	/* Configures the ADC DMA */
  ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);
  /* Enable the ADC DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  
  /* wait for ADRDY */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
  	  
	/* Enable the DMA channel */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  /* Start ADC1 Software Conversion */ 
  ADC_StartConversion(ADC1);   
 	
}

void Init_USART(void)
{
  NVIC_InitTypeDef      NVIC_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);//RX USART2 PA3
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);//TX USART2 PA2
	
  /* Configure USART1 pins:  Rx and Tx ----------------------------*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Activation IT en reception*/
  /* Enable USART1 IRQ */

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  USART_InitStructure.USART_BaudRate =115200; //
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;// 8 bits ou 9 possible
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;// activation des lignes

  USART_Init(USART2, &USART_InitStructure);
  
  USART_Cmd(USART2,ENABLE);// activation USART2
	
	//	 Dma configuration pour USART	
	// 	 DMA_InitTypeDef  DMA_InitStructure;

  DMA_DeInit(DMA1_Channel7);

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST ; // Emission
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TX_USART;//TX_USART
  DMA_InitStructure.DMA_BufferSize = (uint16_t)sizeof(TX_USART);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->TDR);// Emission 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal  ;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

  DMA_Init(DMA1_Channel7, &DMA_InitStructure);

  /* Enable the USART Tx DMA request */
  USART_DMACmd(USART2, USART_DMAReq_Tx , ENABLE);	
	
	/* Activation reception ITM enum RX_USART*/
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 

  /* Enable DMA Stream Transfer Complete interrupt */
	// DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
 
  /* Enable the DMA TX Stream */
	// DMA_Cmd(DMA1_Channel7, ENABLE);// start prmière fois

	/* Restart DMA en mode normal : procedure */
	/* DMA1_Channel7->CNDTR = (uint16_t)sizeof(TX_USART) idem DMA_SetCurrDataCounter(DMA1_Channel7, sizeof(TX_USART))*/

	// DMA_Cmd(DMA1_Channel7, DISABLE);// arret
	// DMA_SetCurrDataCounter(DMA1_Channel7, sizeof(TX_USART));//mise au depart pointeur sur Buffer en arret DMA
	// DMA_Cmd(DMA1_Channel7, ENABLE);//start envoi buffer 

}