/**
  ******************************************************************************
  * @file    Init.h
  * @author  INSA Toulouse
  * @version V1.0
  * @date    13-Juin-2017
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include "platform_config.h"
#include "stm32f30x_gpio.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "usb_lib.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SHDNM1 GPIO_Pin_6 //shutdown moteur 1 PC6
#define SHDNM2 GPIO_Pin_7 //shutdown moteur 1 PC7
#define CMD_ALIM GPIO_Pin_8//Commande alim générale
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

void Init_shutdown(void);
void Init_IT_EXT(void);
void Init_IT_DMA1(void);
void Init_ADC1_DMA1(void);
void Init_USART(void);