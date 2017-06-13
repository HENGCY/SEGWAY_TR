/**
  ******************************************************************************
  * @file    main.h 
  * @author  INSA Toulouse
  * @version V1.0
  * @date    13-Juin-2017
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include <stdio.h>
#include "stm32f3_discovery_lsm303dlhc.h"
#include "stm32f3_discovery_l3gd20.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "platform_config.h"
#include "math.h"
#include "stm32f0xx_lcd.h"
#include "stm32f30x_gpio.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Demo_USB (void);
void Demo_GyroConfig(void);
void Demo_GyroReadAngRate (float* pfData);
void USARTINIT(void);
void USARTstring( const unsigned char *LCD_chaine);   //Outputs string to USART
void Init_ADC1_DMA1(void);
void IT_EXT_Init(void);
void IT_DMA1_Init(void);
void Trait_Gyro_Acc (void);
void Demo_CompassConfig(void);
void Demo_CompassReadMag (float* pfData);
void Demo_CompassReadAcc(float* pfData);
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
void TIM_Config(void);
void ShutdownInit(void);



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
