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


/* Private define ------------------------------------------------------------*/
#define SHDNM1 GPIO_Pin_6 						//	Shutdown moteur 1 PC6
#define SHDNM2 GPIO_Pin_7 						//	Shutdown moteur 1 PC7
#define CMD_ALIM GPIO_Pin_8						//	Commande alim générale
#define facteur_prescaler 1 					// 	Facteur pour TIMER 15
#define Freq_PWM 25000								// 	Frequence PWM en Hz  
#define Freq_SYS 72000000							// 	Frequence systeme
#define PWM50  (Freq_SYS/(2*Freq_PWM))// 	Initielisation compteur Timer pour 50 %


/* Private structure ---------------------------------------------------------*/
//structure pour initialisation et configuration de peripheriques
extern RCC_ClocksTypeDef RCC_Clocks;
extern GPIO_InitTypeDef  GPIO_InitStructure;	
extern USART_InitTypeDef USART_InitStructure;
extern ADC_InitTypeDef       ADC_InitStructure;
extern ADC_CommonInitTypeDef ADC_CommonInitStructure;
extern DMA_InitTypeDef DMA_InitStructure;
extern TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
extern TIM_OCInitTypeDef  TIM_OCInitStructure;
	
/* Private variable ---------------------------------------------------------*/

//Variable pour ADC
extern __IO uint16_t Acq_ADC1[6];
extern __IO uint16_t calibration_value;

//Variable pour USART
extern uint8_t TX_USART [46];							//	Buffer emission
extern uint8_t RX_USART [10];							//	Buffer reception
extern uint8_t ST[2] ; 										//	Buffer status

//Variable pour Timer Et PWM
extern uint16_t TimerPeriod; 
extern uint16_t PWM_MOTEUR1;
extern uint16_t PWM_MOTEUR2; 



/* Exported functions ------------------------------------------------------- */
void Init_shutdown(void);
void Init_IT_EXT(void);
void Init_IT_DMA1(void);
void Init_ADC1_DMA1(void);
void Init_USART(void);
void TIM_Config(void);


#endif /* __INIT_H */

