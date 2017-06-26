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


__IO uint32_t  ADC1ConvertedValue = 0, ADC1ConvertedVoltage = 0;
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

__IO uint32_t TimingDelay = 0, toto;

/*Correcteur PI du courant */
float Consigne1,Consigne2;
float v1_k=0.0,v2_k=0.0;
float u1_k=0.0,u2_k=0.0;
float err1=0.0, err2=0.0, err1_a=0.0, err2_a=0.0;
float y1_k=0.0,y2_k=0.0, y1_k_1=0,y2_k_1=0;
float i1=0.0; // intégrateur
float i2=0.0;
float new_i_1=0.0;
float new_i_2=0.0;
bool int1_ok;
bool int2_ok;
float i1_k, i1_k_1=0.0, i2_k, i2_k_1=0.0; 
float a1;
float a0;
float test1 = 0 ; 
float test2 = 0 ;
int cmpt=0;


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
	IM1 = (int32_t)(0.96 * COEFI1 * ((float) (ADC1ConvertedValue *ScaleADC/0xFFF) - VIM10));
			
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

	IM2 = (int32_t)(0.96 * COEFI2 * ((float) (ADC1ConvertedValue *ScaleADC/0xFFF)- VIM20));//Unité X 1/100 A
	
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
											
 /* Affichage vitesse angulaire en °/s*/	
 /*M4*/	
 //	VitesseAngle = (Zerogyro+Buffer[0]);// vitesse angulaire
       //Xval = ABS((int16_t)(10*VitesseAngle));// en dizaine de degré/s
			 Xval = ABS((int16_t)(y2_k*100));// en dizaine de degré/s
			if (Xval < 50) 
				TX_USART [35] ='-';
			else
				TX_USART [35] ='+';
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
						TX_USART [45]= '\r';//RC*/
	/*Envoi DMA TX USART2*/ 
	DMA_Cmd(DMA1_Channel7, DISABLE);// arret
	DMA_SetCurrDataCounter(DMA1_Channel7, sizeof(TX_USART));//mise au depart pointeur sur Buffer en arret DMA
	DMA_Cmd(DMA1_Channel7, ENABLE);//start envoi buffer 

	/*********************************************************************/
	/*                            ZONE CALCUL                            */
	/*********************************************************************/

	//GPIO_SetBits(GPIOC,SHDNM1 | SHDNM2);// moteurs 1&2 ON

/*
	if (((ST[0]<58) && (ST[0]>47))&&((ST[1]<58) && (ST[1]>47)))// test 00 à 99 ASCII
	{
		Tampon1 =(2* PWM50*( (ST[0]-48)*10 +  ST[1]-48))/100;// conversion en %
		PWM_MOTEUR1 = Tampon1;
		PWM_MOTEUR2 = Tampon1;

		TIM_SetCompare1( TIM1,  PWM_MOTEUR1);// ecriture de CCR1 de 16 bits
		TIM_SetCompare2( TIM1,  PWM_MOTEUR2);// ecriture de CCR2 de 16 bits
	}*/

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


void Trait_b_courant (void) {

	//STM_EVAL_LEDOn(LED6);	
	ADC1ConvertedValue =  MOY_ADC12_IN6;// resultat en mv 3000 max
	IM1 = (int32_t)(0.96 * COEFI1 * ((float) (ADC1ConvertedValue *ScaleADC/0xFFF) - VIM10));
					
	ADC1ConvertedValue =  MOY_ADC12_IN7;// resultat en mv 3000 max
	IM2 = (int32_t)(0.96 * COEFI2 * ((float) (ADC1ConvertedValue *ScaleADC/0xFFF)- VIM20));//Unité X 1/100 A
			
	GPIO_SetBits(GPIOC,SHDNM1 | SHDNM2);// moteurs 1&2 ON
	
	//consigne en ampère
	if(cmpt<10000){
			Consigne1= 2.0;
			Consigne2= 2.0;
		}
	else{ 
		Consigne1= 0.0;
		Consigne2= 0.0;
	}


/******* Correcteur Gain Proportionnel OK ***************************************/
// 	err1 = (Consigne1 - (float)IM1/100.0); // err dans unité de courant 
//   err2 = (Consigne2 - (float)IM2/100.0);

// 	v1_k =  Kp * err1; 	//	vi_k dans unité de tension 
// 	v2_k =  Kp * err2;	//	Kp en unité dé V/A
// 	
// 	u1_k= v1_k/48.0+0.5;
// 	u2_k= v2_k/48.0+0.5;
// 	
// 	if (u1_k >0.9)
// 		u1_k=0.9;
// 	else if (u1_k<0.1)
// 		u1_k=0.1;

// 	if (u2_k >0.9)
// 		u2_k=0.9;
// 	else if (u2_k<0.1)
// 		u2_k=0.1;
// 	
// 	PWM_MOTEUR1 = (2.0* PWM50*(u1_k));// conversion en %
// 	PWM_MOTEUR2 = (2.0* PWM50*(u2_k));// conversion en %
// 	
// 	TIM_SetCompare1( TIM1,  PWM_MOTEUR1);// ecriture de CCR1 de 16 bits
// 	TIM_SetCompare2( TIM1,  PWM_MOTEUR2);// ecriture de CCR2 de 16 bits	

// 	STM_EVAL_LEDOff(LED6);


/******* Correcteur Gain Proportionnel Integral OK ***************************************/
	err1 = (Consigne1 - (float)IM1/100.0); // err dans unité de courant 
  err2 = (Consigne2 - (float)IM2/100.0);

	a1=Kp*(Te/(2.0*Ti)+1.0);
	a0=Kp*(Te/(2.0*Ti)-1.0);
	
	y1_k=(y1_k_1+a1*err1+a0*err1_a) ;
	y2_k=(y2_k_1+a1*err2+a0*err2_a) ;
	
	//Saturateur
	if (y1_k >24.0)
		y1_k=24.0;
	 else if (y1_k<-24.0)
		y1_k=-24.0;

	if (y2_k >24.0)
		y2_k=24.0;
	else if (y2_k<-24.0)
		y2_k=-24.0;
	
	u1_k= y1_k/48.0+0.5;
	u2_k= y2_k/48.0+0.5;
	
	
	//==== Décalage entrée sortie
	err1_a=err1;
	err2_a=err2;
	
	y1_k_1=y1_k;
	y2_k_1=y2_k;

	// Limitation sortie
	if (u1_k >0.9)
		u1_k=0.9;
	else if (u1_k<0.1)
		u1_k=0.1;

	if (u2_k >0.9)
		u2_k=0.9;
	else if (u2_k<0.1)
		u2_k=0.1;
	
	PWM_MOTEUR1 = (2.0* PWM50*(u1_k));// conversion en %
	PWM_MOTEUR2 = (2.0* PWM50*(u2_k));// conversion en %
	
	TIM_SetCompare1( TIM1,  PWM_MOTEUR1);// ecriture de CCR1 de 16 bits
	TIM_SetCompare2( TIM1,  PWM_MOTEUR2);// ecriture de CCR2 de 16 bits	
	
	STM_EVAL_LEDOff(LED6);
	/*********** PI Anti-windup PAS OK ***************/
// 	new_i_1 = i1 + err1;
// 	new_i_2 = i2 + err2;
// 	
// 	y1_k = Kp * err1 + Ki * new_i_1;
// 	y2_k = Kp * err2 + Ki * new_i_2;

// 	int1_ok = true;
// 	int2_ok = true;
// 	
// 	if (y1_k > 1.0){
// 		y1_k = 1.0;
// 		if (err1 > 0.0){
// 			int1_ok = false; // Anti-windup
// 		}
// 	}
// 	
// 	if (y1_k < -1.0){
// 		y1_k = -1.0;
// 		if (err1 < 0.0){
// 			int1_ok = false;
// 		}
// 	}
// 	
// 	if (y2_k > 1.0){
// 		y2_k = 1.0;
// 		if (err2 > 0.0){
// 			int2_ok = false;
// 		}
// 	}
// 	
// 	if (y2_k < -1.0){
// 		y2_k = -1.0;
// 		if (err2 < 0.0){
// 			int1_ok = false;
// 		}
// 	}
// 	
// 	if (int1_ok) i1 = new_i_1;
// 	if (int2_ok) i2 = new_i_2;
// 	
// 	y1_k = 0.5*y1_k + 0.5;	// Normalisation
// 	y2_k = 0.5*y2_k + 0.5;


	
	/************** PI Bilinéaire PAS OK *****************/
	
// 	a1=Kp*(Te/(2.0*Ti)+1.0);
// 	a0=Kp*(Te/(2.0*Ti)-1.0);
// 	
// 	/*Moteur 1 et 2*/
// 	err1=(Consigne1-(float)IM1/100.0)/30.0; // divise par trente pour normaliser
// 	err2=(Consigne2-(float)IM2/100.0)/30.0;

// 	/*bilineare*/
// 	//PI courant, alpha= sortie de correcteur
//  	
// 	y1_k=(y1_k_1+a1*err1+a0*err1_a) ;
//   y2_k=(y2_k_1+a1*err2+a0*err2_a) ;
// 	
// 	if (y1_k >1.0)
// 		y1_k=1.0;
// 	else if (y1_k<-1.0)
// 		y1_k=-1.0;

// 	if (y2_k >1.0)
// 		y2_k=1.0;
// 	else if (y2_k<-1.0)
// 		y2_k=-1.0;

// 	test1=(0.5*y1_k+0.5);
// 	test2=(0.5*y2_k+0.5);
// 	
// 	//Saturateur
// 	if (test1 >0.9)
// 		test1=0.9;
// 	else if (test1<0.1)
// 		test1=0.1;

// 	if (test2 >0.9)
// 		test2=0.9;
// 	else if (test2<0.1)
// 		test2=0.1;
// 	
// 	err1_a=err1;
// 	err2_a=err2;
// 	y1_k_1=y1_k;
// 	y2_k_1=y2_k;
// 	
// 	PWM_MOTEUR1 = (2.0* PWM50*(test1));// conversion en %
// 	PWM_MOTEUR2 = (2.0* PWM50*(test2));// conversion en %
	
	//TIM_SetCompare1( TIM1,  PWM_MOTEUR1);// ecriture de CCR1 de 16 bits
	//TIM_SetCompare2( TIM1,  PWM_MOTEUR2);// ecriture de CCR2 de 16 bits	
	
		/**********************************************************************************antiwindup****************************************************2206*/
	/********************PI_v2********/
	
// 	err1 = (Consigne1 - (float)IM1/100.0); // Erreur de courant 
//   err2 = (Consigne2 - (float)IM2/100.0);
// 	
// 	
// 	i1_k = err1 * Ki * Te - i1_k_1;
// 	i2_k = err2 * Ki * Te - i2_k_1;
// 	
// 	if(i1_k > 24.0){
// 		i1_k = 24.0;
// 	}
// 	
// 	if(i1_k < -24.0){
// 		i1_k = -24.0;
// 	}
// 	
// 	if(i2_k > 24.0){
// 		i2_k = 24.0;
// 	}
// 	
// 	if(i2_k < -24.0){
// 		i2_k = -24.0;
// 	}
// 		
// 	i1_k_1 = i1_k;
// 	i2_k_1 = i2_k;
// 	
// 	u1_k = i1_k + Kp*err1;		
// 	u2_k = i2_k + Kp*err2;
// 	
// 	if(u1_k > 24.0 && err1 > 0.0){
// 			i1_k = i1_k_1;
// 	}
// 	
// 	if(u1_k < -24.0 && err1 < 0.0){
// 			i1_k = i1_k_1;
// 	}
// 	
// 	if(u2_k > 24.0 && err2 > 0.0){
// 			i2_k = i2_k_1;
// 	}
// 	
// 	if(u2_k < -24.0 && err2 < 0.0){
// 			i2_k = i2_k_1;
// 	}
// 	
// 	u1_k = v1_k/48.0 + 0.5;
// 	u2_k = v2_k/48.0 + 0.5;
// 	
// 		// Limitation sortie
// 	if (u1_k >0.9)
// 		u1_k=0.9;
// 	else if (u1_k<0.1)
// 		u1_k=0.1;

// 	if (u2_k >0.9)
// 		u2_k=0.9;
// 	else if (u2_k<0.1)
// 		u2_k=0.1;
// 	
// 	PWM_MOTEUR1 = (2.0* PWM50*(u1_k));// conversion en %
// 	PWM_MOTEUR2 = (2.0* PWM50*(u2_k));// conversion en %
// 	
// 	TIM_SetCompare1( TIM1,  PWM_MOTEUR1);// ecriture de CCR1 de 16 bits
// 	TIM_SetCompare2( TIM1,  PWM_MOTEUR2);// ecriture de CCR2 de 16 bits	
/********************************************************************************************************************************/
	
// 	if (u1_k > 0.9)
// 		u1_k = 0.9;
// 	else if (u1_k < 0.1)
// 		u1_k = 0.1;

// 	if (u2_k > 0.9)
// 		u2_k = 0.9;
// 	else if (u2_k < 0.1)
// 		u2_k = 0.1;
// 	
// 	PWM_MOTEUR1 = (2.0* PWM50*(u1_k));// conversion en %
// 	PWM_MOTEUR2 = (2.0* PWM50*(u2_k));// conversion en %
// 	
// 	TIM_SetCompare1( TIM1,  PWM_MOTEUR1);// ecriture de CCR1 de 16 bits
// 	TIM_SetCompare2( TIM1,  PWM_MOTEUR2);// ecriture de CCR2 de 16 bits	
// 	
// 	STM_EVAL_LEDOff(LED9);	
// 	STM_EVAL_LEDOff(LED6);	

// 	a0=(Te + 2.0*Ti)/(2.0*Tau_2);
// 	a1=(Te - 2.0*Ti)/(2.0*Tau_2);	
// 	err1 = (Consigne1 - (float)IM1/100.0)/30.0; // Normalisation
//   err2 = (Consigne2 - (float)IM2/100.0)/30.0;
// 	
// 	y1_k=(y1_k_1+a1*err1+a0*err1_a) ;
// 	y2_k=(y2_k_1+a1*err2+a0*err2_a) ;
// 	
// 		//Saturateur
// 	if (y1_k >24.0)
// 		test1=24.0;
// 	 else if (y1_k<-24.0)
// 		y1_k=-24.0;

// 	if (y2_k >24.0)
// 		y2_k=24.0;
// 	else if (y2_k<24.0)
// 		y2_k=-24.0;
// 	
// 	u1_k= v1_k/48.0+0.5;
// 	u2_k= v2_k/48.0+0.5;
// 	
// 	
// 	//==== Décalage entrée sortie
// 	err1_a=err1;
// 	err2_a=err2;
// 	
// 	y1_k_1=y1_k;
// 	y2_k_1=y2_k;

	// Limitation sortie
// 	if (u1_k >0.9)
// 		u1_k=0.9;
// 	else if (u1_k<0.1)
// 		u1_k=0.1;

// 	if (u2_k >0.9)
// 		u2_k=0.9;
// 	else if (u2_k<0.1)
// 		u2_k=0.1;
// 	
// 	PWM_MOTEUR1 = (2.0* PWM50*(u1_k));// conversion en %
// 	PWM_MOTEUR2 = (2.0* PWM50*(u2_k));// conversion en %
// 	
// 	TIM_SetCompare1( TIM1,  PWM_MOTEUR1);// ecriture de CCR1 de 16 bits
// 	TIM_SetCompare2( TIM1,  PWM_MOTEUR2);// ecriture de CCR2 de 16 bits	

	cmpt++;
	
	if(cmpt==20000) cmpt=0;

}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}
/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/************************ (C) COPYRIGHT INSA TOULOUSE *****END OF FILE****/
