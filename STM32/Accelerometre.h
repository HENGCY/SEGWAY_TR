/**
  ******************************************************************************
  * @file    Accelerometre.h
  * @author  INSA Toulouse
  * @version V1.0
  * @date    13-Juin-2017
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include "stm32f3_discovery_l3gd20.h"
#include "math.h"
#include "stm32f30x_gpio.h"
#include "hw_config.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LSM_Acc_Sensitivity_2g     (float)     1.0f            /*!< accelerometer sensitivity with 2 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_4g     (float)     0.5f            /*!< accelerometer sensitivity with 4 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_8g     (float)     0.25f           /*!< accelerometer sensitivity with 8 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_16g    (float)     0.0834f         /*!< accelerometer sensitivity with 12 g full scale [LSB/mg] */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

void Acc_read(float* pfData);
void Acc_config(void);
uint32_t Acc_TIMEOUT_UserCallback(void);