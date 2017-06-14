/**
  ******************************************************************************
  * @file    Fonctions.c  
  * @author  INSA Toulouse
  * @version V1.0 
  * @date    13-Juin-2017 
  * @brief    
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "Fonctions.h"

//Test
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

/**
  * @brief  Traitement données de Gyro, Accelero et ADC
  * @param   
  * @retval  
  */
void Trait_Gyro_Acc (void) {
	
	//Variables calcul
	uint16_t Tampon1 = 0 ; 

  /* Read Gyro Angular data */
	Gyro_read(Buffer);
	
  /* Read Accelero */
	Acc_read(AccBuffer);
	
	// compensation offset axe X A figer !!!!!!!
	AccBuffer[0] = AccBuffer[0] + ZeroACCX;	
			
	/* angle calcul accelero*/		
	/* bornage*/
	AccBuffer[2] = ABS (AccBuffer[2]);// Acceleration z ne peut etre négative sur gyropode phase change de signe 
	
	/*calcul angle a patir de AccX et AccZ*/	
 	Angleacc  =-atan (AccBuffer[0]/AccBuffer[2])*180/PI ;// - atan( AccX/AccZ)X 180/PI

	/*Mesure Angle + filtrage complémentaire en °et  X 10  résolution 0.1 °*/
	/*M0*/ 
	Anglemesure	= 0.98 *(Anglemesure	+	(Deltatps * (Zerogyro+Buffer[0])))	+0.02*Angleacc;//0.98 + 0.02 = 1
	VitesseAngle = (Zerogyro+Buffer[0]);// vitesse angulaire
  Xval = ABS((int16_t)(10*Anglemesure));// en dixiaime de degré et X 10
	if (AccBuffer[0] < 0) // signe à revoir tester le signe  de Anglemesure !!!!!!!!!!!!
		TX_USART [3] ='+';
	else
		TX_USART [3] ='-';
    TX_USART [0] = 'M';
		TX_USART [1] = '0';
		TX_USART [2] = ':';
		TX_USART [4] = bintoascii(Xval)[1];//100 aine
		TX_USART [5] = bintoascii(Xval)[2];//10 aine
		TX_USART [6] = bintoascii(Xval)[3];//unité
		TX_USART [7] = bintoascii(Xval)[4];//dixieme

	/* Mesure courant Moteur 1 + mise echelle PC0 en Ampére et X 100 résolution 0.01 Ampere */
	/*M1*/			
	ADC1ConvertedValue =  MOY_ADC12_IN6;// resultat en mv 3000 max
	//	=0.96 (V-V1M10) X COEF1
	IM1 = (uint32_t)(0.96 * COEFI1 * ((float) (ADC1ConvertedValue *ScaleADC/0xFFF) - VIM10));
			
	if (IM1 < 0)		
		{TX_USART [11] ='-';// signe à traiter
	  IM1 =ABS(IM1);}
	else
		TX_USART [11] ='+';
		
	ADC1ConvertedVoltage = 	(uint16_t) IM1;	
  
						TX_USART [8] = 'M';
						TX_USART [9] = '1';
						TX_USART [10] = ':';
						TX_USART [12]= bintoascii(ADC1ConvertedVoltage)[1];//100 aine
						TX_USART [13]= bintoascii(ADC1ConvertedVoltage)[2];//10 aine
						TX_USART [14]= bintoascii(ADC1ConvertedVoltage)[3];//unité
						TX_USART [15]= bintoascii(ADC1ConvertedVoltage)[4];//dixieme
		 
	/* Mesure courant Moteur 2 + mise echelle PC1 en Ampére X 100 résolution 0.01 Ampere */
	/*M2*/		
	ADC1ConvertedValue =  MOY_ADC12_IN7;// resultat en mv 3000 max
	//			=0.96 (V-V1M20) X COEF2

	IM2 = (uint32_t)(0.96 * COEFI2 * ((float) (ADC1ConvertedValue *ScaleADC/0xFFF)- VIM20));//Unité X 1/100 A
	
	if (IM2 < 0)		
		{TX_USART [19] ='-';// signe à traiter
	  IM2 =ABS(IM2);}
	else
		TX_USART [19] ='+';
		
	ADC1ConvertedVoltage = 	(uint16_t) IM2;	
            TX_USART [16] = 'M';
			      TX_USART [17] = '2';
            TX_USART [18] = ':';
						TX_USART [20]= bintoascii(ADC1ConvertedVoltage)[1];//100 aine
						TX_USART [21]= bintoascii(ADC1ConvertedVoltage)[2];//10 aine
						TX_USART [22]= bintoascii(ADC1ConvertedVoltage)[3];//unité
						TX_USART [23]= bintoascii(ADC1ConvertedVoltage)[4];//dixieme

	
 /* Mesure jauge Guidon  PC2*/
	/*M3*/			
	ADC1ConvertedValue =  MOY_ADC12_IN8;//IN8
  ADC1ConvertedVoltage = (ADC1ConvertedValue *ScaleADC)/0xFFF;
            TX_USART [24] = 'M';
			      TX_USART [25] = '3';
            TX_USART [26] = ':';
            TX_USART [27] ='+';// signe à traiter
						TX_USART [28]= bintoascii(ADC1ConvertedVoltage)[1];//100 aine
						TX_USART [29]= bintoascii(ADC1ConvertedVoltage)[2];//10 aine
						TX_USART [30]= bintoascii(ADC1ConvertedVoltage)[3];//unité
						TX_USART [31]= bintoascii(ADC1ConvertedVoltage)[4];//dixieme
 
 /* Mesure tension batterie PC3 MOY_ADC12_IN9     */
 /* Tension bat divisée par 19 en mV */
 /*M4*/	
 // Affichage en M4  de la tension de bat désactivée
	/*
	
	ADC1ConvertedValue =  MOY_ADC12_IN9;// decommenter
  ADC1ConvertedVoltage = (ADC1ConvertedValue *ScaleADC)/0xFFF;
            TX_USART [32] = 'M';
			      TX_USART [33] = '4';
            TX_USART [34] = ':';
            TX_USART [35] ='+';// signe à traiter
						TX_USART [36]= bintoascii(ADC1ConvertedVoltage)[1];//100 aine
						TX_USART [37]= bintoascii(ADC1ConvertedVoltage)[2];//10 aine
						TX_USART [38]= bintoascii(ADC1ConvertedVoltage)[3];//unité
						TX_USART [39]= bintoascii(ADC1ConvertedVoltage)[4];//dixieme
						
		*/				
						
 /* Affichage vitesse angulaire en °/s*/	
 /*M4*/	 
 	VitesseAngle = (Zerogyro+Buffer[0]);// vitesse angulaire
       Xval = ABS((int16_t)(10*VitesseAngle));// en dizaine de degré/s
			if (VitesseAngle < 0) 
				TX_USART [35] ='+';
			else
				TX_USART [35] ='-';
            TX_USART [32] = 'M';
			      TX_USART [33] = '4';
            TX_USART [34] = ':';
						TX_USART [36] = bintoascii(Xval)[1];//100 aine
						TX_USART [37] = bintoascii(Xval)[2];//10 aine
						TX_USART [38] = bintoascii(Xval)[3];//unité
						TX_USART [39] = bintoascii(Xval)[4];//dixieme

						
						
/* STATUS */
            TX_USART [40] = 'S';
			      TX_USART [41] = 'T';
            TX_USART [42] = ':';
            TX_USART [43] =ST[0];// 0
            TX_USART [44] =ST[1];// 0
						TX_USART [45]= '\r';//RC
	/*Envoi DMA TX USART2*/ 
	DMA_Cmd(DMA1_Channel7, DISABLE);// arret
	DMA_SetCurrDataCounter(DMA1_Channel7, sizeof(TX_USART));//mise au depart pointeur sur Buffer en arret DMA
	DMA_Cmd(DMA1_Channel7, ENABLE);//start envoi buffer 

	/*********************************************************************/
	/*                            ZONE CALCUL                            */
	/*********************************************************************/

	GPIO_SetBits(GPIOC,SHDNM1 | SHDNM2);// moteurs 1&2 ON


	if (((ST[0]<58) && (ST[0]>47))&&((ST[1]<58) && (ST[1]>47)))// test 00 à 99 ASCII
	{
		Tampon1 =(2* PWM50*( (ST[0]-48)*10 +  ST[1]-48))/100;// conversion en %
		PWM_MOTEUR1 = Tampon1;
		PWM_MOTEUR2 = Tampon1;

		TIM_SetCompare1( TIM1,  PWM_MOTEUR1);// ecriture de CCR1 de 16 bits
		TIM_SetCompare2( TIM1,  PWM_MOTEUR2);// ecriture de CCR2 de 16 bits
	}

	/*********************************************************************/	
  STM_EVAL_LEDOff(LED10);//Test fin de calcul
	/*********************************************************************/	
}



/**
  * @brief  Transmission USART avec LF RC
  * @param   
  * @retval  
  */
void USARTstring( const unsigned char *LCD_chaine)
{   //Outputs string to USART
//    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)  == RESET);
//   
//   LF RC
//   while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)  == RESET);
//  		
// 	USART_SendData(USART2,'\t');// transmission data	
// 	  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)  == RESET);
//  		
// 	USART_SendData(USART2,'\r');// transmission data	
// 	USART_SendData(USART2,'\n');// transmission data	

while (*LCD_chaine)
	{
   while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)  == RESET);
			
		USART_SendData(USART2, *LCD_chaine);// transmission data
			
	  LCD_chaine++;
	}


}


/************************ (C) COPYRIGHT INSA TOULOUSE *****END OF FILE****/
