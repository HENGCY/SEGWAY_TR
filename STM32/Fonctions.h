/**
  ******************************************************************************
  * @file    Fonctions.h 
  * @author  INSA Toulouse
  * @version V1.0
  * @date    13-Juin-201
  * @brief   Header for Fonctions.c
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONCTIONS_H
#define __FONCTIONS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include "Gyroscope.h"
#include "Accelerometre.h"
#include "Init.h"
#include "math.h"
#include "stm32f0xx_lcd.h"



/* Private define ------------------------------------------------------------*/
#define PI                         (float)     3.14159265f 
#define Deltatps                   (float)     0.010526f         /*!< Delta t du Gyro  = 1/95 HZ */
#define ScaleADC                    3000                        /*pleine echelle ADC 3V */
#define Freq_PWM 25000// frequence PWM en Hz  
#define Freq_SYS 72000000// frequence systeme
#define PWM50  (Freq_SYS/(2*Freq_PWM))// initielisation compteur Timer pour 50 %
#define SHDNM1 GPIO_Pin_6 //shutdown moteur 1 PC6
#define SHDNM2 GPIO_Pin_7 //shutdown moteur 1 PC7
#define COEFI1  (float) 1.71283f // attenuateur capteur courant moteur 1  = 2.523/1.473 sortie capteur/ entrée mesure à vide in ADC
#define COEFI2  (float) 1.71584f // attenuateur capteur courant moteur 2  = 2.512/1.464 sortie capteur/ entrée mesure à vide in ADC

/* Global variables ---------------------------------------------------------*/


extern __IO uint32_t  ADC1ConvertedValue , ADC1ConvertedVoltage;
extern __IO uint16_t MOY_ADC12_IN6 , MOY_ADC12_IN7 , MOY_ADC12_IN8 , MOY_ADC12_IN9 ,MOY_ADC1_IN2 ,MOY_ADC1_IN5 ;;// variables lectures ADC moyennees de Acq_ADC1[4]
extern __IO int32_t  IM1,IM2,VIM10,VIM20;

extern float ZeroACCX ;// zero accelero X
extern float Zerogyro ;// zero du gyro
extern float Angleacc ;// calcule de l'angle a partir de tangente inverse
extern uint16_t Xval, Yval, Zval;
extern uint16_t Angleval ;

// __IO float HeadingValue = 0.0f;  
extern float MagBuffer[3], AccBuffer[3], Buffer[3];
extern float Anglegyro;// calcule de l'angle a partir de d °/seconde
extern float Anglemesure;// calcule de l'angle fusionné
extern float VitesseAngle;// vitesse angulaire

//Reserve
/*

__IO uint32_t  ADC1ConvertedValue = 0, ADC1ConvertedVoltage = 0;
//__IO uint16_t Acq_ADC1[6], calibration_value = 0;
__IO uint16_t MOY_ADC12_IN6 = 0, MOY_ADC12_IN7 = 0, MOY_ADC12_IN8 = 0, MOY_ADC12_IN9 =0,MOY_ADC1_IN2 =0,MOY_ADC1_IN5 =0;;// variables lectures ADC moyennees de Acq_ADC1[4]
__IO int32_t  IM1,IM2,VIM10,VIM20;

float ZeroACCX = 0.0f;// zero accelero X
float Zerogyro = 0.0f;// zero du gyro
float Angleacc = 0.0f;// calcule de l'angle a partir de tangente inverse
uint16_t Xval, Yval, Zval = 0x00;
uint16_t Angleval = 0x00;

// __IO float HeadingValue = 0.0f;  
float MagBuffer[3] = {0.0f}, AccBuffer[3] = {0.0f}, Buffer[3] = {0.0f};
float Anglegyro = 0.0f;// calcule de l'angle a partir de d °/seconde
float Anglemesure = 0.0f;// calcule de l'angle fusionné
float VitesseAngle = 0.0f;// vitesse angulaire
*/


/* Private macro -------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x


/* Exported functions ------------------------------------------------------- */
void Trait_Gyro_Acc (void);
void USARTstring( const unsigned char *LCD_chaine);

#endif /* __FONCTIONS_H */
/************************ (C) COPYRIGHT INSA TOULOUSE ************************/
