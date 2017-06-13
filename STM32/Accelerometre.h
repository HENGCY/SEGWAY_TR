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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

void Acc_read(float* pfData);
void Acc_config(void);
uint32_t Acc_TIMEOUT_UserCallback(void);