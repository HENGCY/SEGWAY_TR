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
#define PWM50  (Freq_SYS/(2*Freq_PWM))// initialisation compteur Timer pour 50 %
#define SHDNM1 GPIO_Pin_6 //shutdown moteur 1 PC6
#define SHDNM2 GPIO_Pin_7 //shutdown moteur 1 PC7
#define COEFI1  (float) 1.71283f // attenuateur capteur courant moteur 1  = 2.523/1.473 sortie capteur/ entrée mesure à vide in ADC
#define COEFI2  (float) 1.71584f // attenuateur capteur courant moteur 2  = 2.512/1.464 sortie capteur/ entrée mesure à vide in ADC


/*Define de coefficient de correcteur de courant */
#define Kp 1.0f
#define Ti 0.0002069f // pour essai sur bloc resistance inductance //0.00094f // L/R, constant du temps de système moteur
//#define Te 1.0f/94
#define Te 0.000342f
#define Tau Ti*10.0f
#define Freq_e  1.0f/Te



/* Global variables ---------------------------------------------------------*/


extern __IO uint32_t  ADC1ConvertedValue , ADC1ConvertedVoltage;
extern __IO uint16_t MOY_ADC12_IN6 , MOY_ADC12_IN7 , MOY_ADC12_IN8 , MOY_ADC12_IN9 ,MOY_ADC1_IN2 ,MOY_ADC1_IN5 ;;// variables lectures ADC moyennees de Acq_ADC1[4]
extern __IO int32_t  IM1,IM2,VIM10,VIM20;

extern float ZeroACCX ;// zero accelero X
extern float Zerogyro ;// zero du gyro
extern float Angleacc ;// calcul de l'angle a partir de tangente inverse
extern uint16_t Xval, Yval, Zval;
extern uint16_t Angleval ;

// __IO float HeadingValue = 0.0f;  
extern float MagBuffer[3], AccBuffer[3], Buffer[3];
extern float Anglegyro;// calcul de l'angle a partir de d °/seconde
extern float Anglemesure;// calcul de l'angle fusionné
extern float VitesseAngle;// vitesse angulaire



/* Private macro -------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x


/* Exported functions ------------------------------------------------------- */
void Trait_Gyro_Acc (void);
void USARTstring( const unsigned char *LCD_chaine);
void Trait_b_courant (void) ;

#endif /* __FONCTIONS_H */
/************************ (C) COPYRIGHT INSA TOULOUSE ************************/
