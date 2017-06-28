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
#include <stdlib.h>

/* Private define ------------------------------------------------------------*/
#define PI                         (float)     3.14159265f 
#define Deltatps                   (float)     0.010526f        /*!< Delta t du Gyro  = 1/95 HZ */
#define ScaleADC                    3000                        /*pleine echelle ADC 3V */
#define Freq_PWM 25000																					//	frequence PWM en Hz  
#define Freq_SYS 72000000																				//	frequence systeme
#define PWM50  (Freq_SYS/(2*Freq_PWM))													//	initialisation compteur Timer pour 50 %
#define SHDNM1 GPIO_Pin_6 																			//	shutdown moteur 1 PC6
#define SHDNM2 GPIO_Pin_7 																			//	shutdown moteur 1 PC7
#define COEFI1  (float) 1.71283f 		// attenuateur capteur courant moteur 1  = 2.523/1.473 sortie capteur/ entrée mesure à vide in ADC
#define COEFI2  (float) 1.71584f 		// attenuateur capteur courant moteur 2  = 2.512/1.464 sortie capteur/ entrée mesure à vide in ADC
#define true 1
#define false 0

/*	Define de coefficient de boucle de courant */
#define R 0.47f
#define L 0.00047f
#define Kp R
#define Ti L/R
#define Ki 1.0f/Ti
#define Freq_e  8500			// Qui correspondant à frequence de IT DMA ADC
#define Te 1.0f/Freq_e

/*	Define de coefficient de commande courant */
#define k1  -25.8598f //-125.5108f   
#define k2 -6.5593f //-60.3998f
    

/* Global variables ---------------------------------------------------------*/

//Variable ADC pour calculer courant 
extern __IO uint32_t  ADC1ConvertedValue , ADC1ConvertedVoltage;
extern __IO uint16_t MOY_ADC12_IN6 , MOY_ADC12_IN7 , MOY_ADC12_IN8 , MOY_ADC12_IN9 ,MOY_ADC1_IN2 ,MOY_ADC1_IN5 ;// variables lectures ADC moyennees de Acq_ADC1[4]
extern __IO int32_t  IM1,IM2,VIM10,VIM20;

//Variable Accelerometre et Gyroscope 
extern float ZeroACCX ;															//	zero accelero X
extern float Zerogyro ;															//	zero du gyro
extern float Angleacc ;															// 	calcule de l'angle a partir de tangente inverse
extern uint16_t Xval;
extern float Anglegyro;															// 	calcule de l'angle a partir de d °/seconde
extern float Anglemesure;														// 	calcule de l'angle fusionné
extern float VitesseAngle;													// 	vitesse angulaire 
extern float MagBuffer[3], AccBuffer[3], Buffer[3];	//	buffer pour acc/gyro


extern float Consigne1;
extern float Consigne2;				//	Consigne de courant de Raspberry Pi
//Variable pour fonction delay
extern __IO uint32_t TimingDelay ;
extern int32_t Consigne;


/* Private macro -------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x


/* Exported functions ------------------------------------------------------- */
void Trait_Gyro_Acc (void);
void Trait_b_courant (void) ;
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void Trait_consigne(void);
unsigned char *bintoascii(uint16_t data_bin);

#endif /* __FONCTIONS_H */
/************************ (C) COPYRIGHT INSA TOULOUSE ************************/
