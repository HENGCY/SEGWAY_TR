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


//#include "Init.h"
//#include "Accelerometre.h"
//#include "Gyroscope.h"
#include "Fonctions.h"
#include <stdio.h>
/*
#include "stm32f30x.h"
#include "stm32f3_discovery.h"

//#include "stm32f3_discovery_lsm303dlhc.h"
//#include "stm32f3_discovery_l3gd20.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "platform_config.h"

//#include "stm32f0xx_lcd.h"
#include "stm32f30x_gpio.h"
*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int main(void);

void assert_failed(uint8_t* file, uint32_t line);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

