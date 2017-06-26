/**
  ******************************************************************************
  * @file    Init.h
  * @author  INSA Toulouse
  * @version V1.0
  * @date    13-Juin-2017
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INIT_H
#define __INIT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include "platform_config.h"
#include "stm32f30x_gpio.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "usb_lib.h"


/* Define ------------------------------------------------------------*/
#define SHDNM1 GPIO_Pin_6 //shutdown moteur 1 PC6
#define SHDNM2 GPIO_Pin_7 //shutdown moteur 1 PC7
#define CMD_ALIM GPIO_Pin_8//Commande alim générale
#define facteur_prescaler 1 // facteur pour TIMER 15
#define Freq_PWM 25000// frequence PWM en Hz  
#define Freq_SYS 72000000// frequence systeme
#define PWM50  (Freq_SYS/(2*Freq_PWM))// initielisation compteur Timer pour 50 %
/*
#define Freq_e 2000 
#define Te 1/Freq_e 
*/

/* Global typedef ---------------------------------------------------------*/
  extern RCC_ClocksTypeDef RCC_Clocks;
	extern GPIO_InitTypeDef  GPIO_InitStructure;	
  extern USART_InitTypeDef USART_InitStructure;
	extern ADC_InitTypeDef       ADC_InitStructure;
  extern ADC_CommonInitTypeDef ADC_CommonInitStructure;
  extern DMA_InitTypeDef DMA_InitStructure;
  extern TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	extern TIM_OCInitTypeDef  TIM_OCInitStructure;
	extern 	NVIC_InitTypeDef nvicStructure;
//	extern	TIM_TimeBaseInitTypeDef timerInitStructure; 
	
/* Definition of Global varaible ---------------------------------------------*/
	extern __IO uint16_t Acq_ADC1[6];
	extern __IO uint16_t calibration_value;

	/* mes déclarations  chaines caracteres a transmettre */	
	extern uint8_t TX_USART [46];
	extern uint8_t lin [17] ;  
	extern uint8_t lin1 [17] ; 
	extern uint8_t RX_USART [10];
	extern uint8_t ST[2] ; 

	//TIMER ET PWM
	extern uint16_t TimerPeriod; 
	extern uint16_t PWM_MOTEUR1;
	extern uint16_t PWM_MOTEUR2; 
	extern __IO uint32_t TimingDelay ;


/* Exported functions ------------------------------------------------------- */
	void Init_shutdown(void);
	void Init_IT_EXT(void);
	void Init_IT_DMA1(void);
	void Init_ADC1_DMA1(void);
	void Init_USART(void);
	void TIM_Config(void);
	void Delay(__IO uint32_t nTime);
	void TimingDelay_Decrement(void);

#endif /* __INIT_H */

/************************ (C) COPYRIGHT INSA TOULOUSE ************************/
