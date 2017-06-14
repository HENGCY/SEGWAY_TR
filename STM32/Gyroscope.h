/**
  ******************************************************************************
  * @file    Gyroscope.h 
  * @author  INSA Toulouse
  * @version V1.0
  * @date    13-Juin-201
  * @brief   Header for Gyroscope.c
  ******************************************************************************
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GYROSCOPE_H
#define __GYROSCOPE_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f30x.h"
//#include "stm32f3_discovery.h"
#include "stm32f3_discovery_l3gd20.h"

//#include "hw_config.h"
//#include "platform_config.h"


/* Private define ------------------------------------------------------------*/
#define L3G_Sensitivity_250dps     (float)   114.285f         /*!< gyroscope sensitivity with 250 dps full scale [LSB/dps] */
#define L3G_Sensitivity_500dps     (float)    57.1429f        /*!< gyroscope sensitivity with 500 dps full scale [LSB/dps] */
#define L3G_Sensitivity_2000dps    (float)    14.285f	      /*!< gyroscope sensitivity with 2000 dps full scale [LSB/dps] */
/* Mettre 250 avec 250 ou 500 avec 500  ou 2000 avec 2000 :*/
#define L3G_Sensitivity L3G_Sensitivity_500dps 
#define L3GD20_SCALE     L3GD20_FULLSCALE_500 // L3GD20_FULLSCALE_250 L3GD20_FULLSCALE_500   L3GD20_FULLSCALE_2000 

/* Private variables ---------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
void Gyro_config(void);
void Gyro_read(float* pfData);
uint32_t L3GD20_TIMEOUT_UserCallback(void);

#endif /* __GYROSCOPE_H */
/************************ (C) COPYRIGHT INSA TOULOUSE ************************/
