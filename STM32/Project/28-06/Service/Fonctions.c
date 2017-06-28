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


// variables lectures ADC moyennes de Acq_ADC1[4]
__IO uint16_t MOY_ADC12_IN6 = 0, MOY_ADC12_IN7 = 0, MOY_ADC12_IN8 = 0, MOY_ADC12_IN9 =0,MOY_ADC1_IN2 =0,MOY_ADC1_IN5 =0;;
__IO uint32_t  ADC1ConvertedValue = 0, ADC1ConvertedVoltage = 0;
__IO int32_t  IM1,IM2,VIM10,VIM20;

uint16_t Xval= 0x00;
float ZeroACCX = 0.0f;			// zero accelero X
float Zerogyro = 0.0f;			// zero du gyro
float Angleacc = 0.0f;			// calcule de l'angle a partir de tangente inverse
float Anglegyro = 0.0f;			// calcule de l'angle a partir de d °/seconde
float Anglemesure = 0.0f;		// calcule de l'angle fusionné
float VitesseAngle = 0.0f;	// vitesse angulaire
float MagBuffer[3] = {0.0f}, AccBuffer[3] = {0.0f}, Buffer[3] = {0.0f};

unsigned char T[6]="";
unsigned char *chain_ascii = T;
__IO uint32_t TimingDelay = 0;


/*Correcteur PI du courant */
/*
*=============================================================================================
* 					MODELE AUTOMATIQUE
*=============================================================================================
*
*
*=====Fonction de transfert du correcteur PI analogique
* C(p) = (Kp*(1+Ti.p))/Ti.p
*
*
*Consigne de courant signé I , est exprimée en I!
*u est le rapport cyclique [0 1 ] 
*
*
*
*	  	(en A)				err(enA)	  ________	  v (en V) 	_________________  u 		  	 ______   Commande (en V)   ______
* Consigne_I-->+( )----------->| Cor_PI |--->--------| v/(2*Ubatt)+0.5 |---->-----| PWM  |-------->----------|Moteur|--->------- I
*	 						 -^     			 	 |________|            |_________________|          |______|                   |______|				|
*							  |																																																					  |
*							  |     ________                                       																											  |
*							  |-<- |RetourI |--<------------------------------------------------------------------------------------------|
*								 		 |________|
*
*Dans notre cas, on a déjà prendre en compte des gains de convertisseur de capteur de courant avec calcul suivant: 
*
*==============ZONE DE CAL LT6  ===================   
*
*V(in ADC) =(1/COEF)  X { V0 +(.625/6) X IM}  formule brute
*V0 sortie capteur à vide , voisine de 2.5 V soit V01 ou V02 pour moteur 1 & 2
*COEF : attenuation du pont diviseur de resitances ( en sortie LTS6 et entree ADC) soit COEF1&2 por moteur 1 & 2
*	 
*	 on pose VIM10 ou VIM20 = V01(2) * 1/COEF1(2) tension mesurée in ADC, à vide
*  V = VIM10(20)+ 1/COEF1(2) X 0.625/6 X IM1(2)) en sortie pont diviseur et entrée ADC
*	 Moteur 1 ou 2 :	
*	 Les tensions mesurées sont en mV de 0 à 3000 mv, dynamique ADC choisi
*	 IM1&2 =(V-VIM10)10^-3 X 9.6 X 100/ COEF1(2) donne format XX.XX  virgule ( X 1/100)
*			=0.96 (V-V1M10)X COEF1(2)
*
*===================================================
*
* Dimensionment PI analog
* Ti = L/R
* Kp = 2*R
*/

float Consigne1=0.0,Consigne2=0.0;				//	Consigne de courant de Raspberry Pi
float v1_k=0.0,v2_k=0.0,v1_k_a=0.0,v2_k_a=0.0;
float u1_k=0.0,u2_k=0.0;
float err1=0.0, err2=0.0, err1_a=0.0, err2_a=0.0;
float a1;
float a0;


/**
  * @brief  Traitement données de Gyro, Accelero et ADC
  * @param   
  * @retval  
  */
void Trait_Gyro_Acc (void) {
	
	Gyro_read(Buffer);												// Read Gyro Angular data 
	Acc_read(AccBuffer);  										// Read Accelero 
	
	AccBuffer[0] = AccBuffer[0] + ZeroACCX;		// compensation offset axe X A figer 
			
	// angle calcul accelero		
	// bornage
	AccBuffer[2] = ABS (AccBuffer[2]);				// Acceleration z ne peut etre négative sur gyropode phase change de signe 
	
	//	calcul angle a patir de AccX et AccZ	
 	Angleacc  =-atan (AccBuffer[0]/AccBuffer[2])*180.0f/PI ;// - atan( AccX/AccZ)X 180/PI

	//Mesure Angle + filtrage complémentaire en °et  X 10  résolution 0.1 °
	//M0 
	Anglemesure	= 0.98f *(Anglemesure	+	(Deltatps * (Zerogyro+Buffer[0])))	+0.02f*Angleacc;//0.98 + 0.02 = 1
	VitesseAngle = (Zerogyro+Buffer[0]);			// vitesse angulaire
  Xval = ABS((int16_t)(10*Anglemesure));		// en dixiaime de degré et X 10
	
	if (AccBuffer[0] < 0) 										// signe à revoir tester le signe  de Anglemesure !!!!!!!!!!!!
		TX_USART [3] ='+';
	else
		TX_USART [3] ='-';
    TX_USART [0] = 'M';
		TX_USART [1] = '0';
		TX_USART [2] = ':';
		TX_USART [4] = bintoascii(Xval)[1];		//100 aine
		TX_USART [5] = bintoascii(Xval)[2];		//10 aine
		TX_USART [6] = bintoascii(Xval)[3];		//unité
		TX_USART [7] = bintoascii(Xval)[4];		//dixieme

	// Mesure courant Moteur 1 + mise echelle PC0 en Ampére et X 100 résolution 0.01 Ampere 
	// M1
	ADC1ConvertedValue =  MOY_ADC12_IN6;
	IM1 = (int32_t)(0.96f * COEFI1 * ((float) (ADC1ConvertedValue *ScaleADC/0xFFF) - VIM10));
			
	if (IM1 < 0)		
		TX_USART [11] ='-';
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
	
	// Mesure courant Moteur 2 + mise echelle PC1 en Ampére X 100 résolution 0.01 Ampere 
	//M2	
	ADC1ConvertedValue =  MOY_ADC12_IN7;
	IM2 = (int32_t)(0.96f * COEFI2 * ((float) (ADC1ConvertedValue *ScaleADC/0xFFF)- VIM20));//Unité X 1/100 A
	if (IM2 < 0)		
		TX_USART [19] ='-';	
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

	
	//Mesure jauge Guidon  PC2
	//M3*/	
	ADC1ConvertedValue =  MOY_ADC12_IN8;//IN8
  ADC1ConvertedVoltage = (ADC1ConvertedValue *ScaleADC)/0xFFF;
	TX_USART [24] = 'M';
	TX_USART [25] = '3';
	TX_USART [26] = ':';
	TX_USART [27] ='+';
	TX_USART [28]= bintoascii(ADC1ConvertedVoltage)[1];//100 aine
	TX_USART [29]= bintoascii(ADC1ConvertedVoltage)[2];//10 aine
	TX_USART [30]= bintoascii(ADC1ConvertedVoltage)[3];//unité
	TX_USART [31]= bintoascii(ADC1ConvertedVoltage)[4];//dixieme
 
	// Mesure tension batterie PC3 MOY_ADC12_IN9 
	// Tension bat divisée par 19 en mV */
	// M4
	//ADC1ConvertedValue =  MOY_ADC12_IN9;
  //ADC1ConvertedVoltage = (ADC1ConvertedValue *ScaleADC)/0xFFF;
	/*
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
 /*M4*/	 /*
 	VitesseAngle = (Zerogyro+Buffer[0]);// vitesse angulaire
  Xval = ABS((int16_t)(10*VitesseAngle));// en dizaine de degré/s
	if (VitesseAngle < 0) 
		TX_USART [35] ='+';
	else*/
	

		if (Consigne1<0.0f)
			TX_USART [35] ='-';
		else    
			TX_USART [35] ='+';
			
		TX_USART [32] = 'M';
		TX_USART [33] = '4';
		TX_USART [34] = ':';
		
		Xval= (uint16_t)(abs(Consigne1*100.0f));
		TX_USART [36] = bintoascii(Xval)[1];//100 aine
		TX_USART [37] = bintoascii(Xval)[2];//10 aine
		TX_USART [38] = bintoascii(Xval)[3];//unité
		TX_USART [39] = bintoascii(Xval)[4];//dixieme
						
	
	// STATUS 
		TX_USART [40] = 'R';
		TX_USART [41] = 'X';
    TX_USART [42] = ':';
    TX_USART [43] =RX_USART[0];// 0
    TX_USART [44] =RX_USART[1];// 0
			
		TX_USART [45] =RX_USART[2];// 0
    TX_USART [46] =RX_USART[3];// 0
			
		TX_USART [47] =RX_USART[4];// 0
    TX_USART [48] =RX_USART[5];// 0
		
		TX_USART [49] =RX_USART[6];// 0

		TX_USART [50]= '\r';//RC*/
		
	//	Envoi DMA TX USART2 
	DMA_Cmd(DMA1_Channel7, DISABLE);													// 	Arret
	DMA_SetCurrDataCounter(DMA1_Channel7, sizeof(TX_USART));	//	Mise au depart pointeur sur Buffer en arret DMA
	DMA_Cmd(DMA1_Channel7, ENABLE);														//	Start envoi buffer 

  STM_EVAL_LEDOff(LED10);//Test fin de calcul
}



/**
  * @brief  Traitement de boucle de courant
  * @param   
  * @retval  
  */
void Trait_b_courant (void) {

	STM_EVAL_LEDOn(LED6);	
	GPIO_SetBits(GPIOC,SHDNM1 | SHDNM2);		// 	moteurs 1&2 ON
	
	//Consigne1= 2.0;
	//Consigne2= 0.0;

	ADC1ConvertedValue =  MOY_ADC12_IN6;		// 	resultat en mv 3000 max
	IM1 = (int32_t)(0.96f * COEFI1 * ((float) (ADC1ConvertedValue *ScaleADC/0xFFF) - VIM10));
					
	ADC1ConvertedValue =  MOY_ADC12_IN7;		//	resultat en mv 3000 max
	IM2 = (int32_t)(0.96f * COEFI2 * ((float) (ADC1ConvertedValue *ScaleADC/0xFFF)- VIM20));
	
/******* Correcteur Gain Proportionnel  ***************************************/
	/*
	err1 = (Consigne1 - (float)IM1/100.0); // err dans unité de courant 
  err2 = (Consigne2 - (float)IM2/100.0);

	v1_k =  Kp * err1; 	//	vi_k dans unité de tension 
	v2_k =  Kp * err2;	//	Kp en unité dé V/A
	
	u1_k= v1_k/48.0+0.5;
	u2_k= v2_k/48.0+0.5;
	
	if (u1_k >0.9)
		u1_k=0.9;
	else if (u1_k<0.1)
		u1_k=0.1;

	if (u2_k >0.9)
		u2_k=0.9;
	else if (u2_k<0.1)
		u2_k=0.1;
	
	PWM_MOTEUR1 = (2.0* PWM50*(Consigne1));// conversion en %
	PWM_MOTEUR2 = (2.0* PWM50*(Consigne2));// conversion en %
	
	TIM_SetCompare1( TIM1,  PWM_MOTEUR1);// ecriture de CCR1 de 16 bits
	TIM_SetCompare2( TIM1,  PWM_MOTEUR2);// ecriture de CCR2 de 16 bits	
*/
/******* Correcteur Gain Proportionnel Integral *******************************/
	err1 = (Consigne1 - (float)IM1/100.0f); // err dans unité de courant 
  err2 = (Consigne2 - (float)IM2/100.0f);

	a1=Kp*(Te/(2.0f*Ti)+1.0f);
	a0=Kp*(Te/(2.0f*Ti)-1.0f);
	
	v1_k=(v1_k_a+a1*err1+a0*err1_a) ;
	v2_k=(v2_k_a+a1*err2+a0*err2_a) ;
	
	//Saturateur
	if (v1_k >24.0f)
		v1_k=24.0f;
	else if (v1_k<-24.0f)
		v1_k=-24.0f;

	if (v2_k >24.0f)
		v2_k=24.0f;
	else if (v2_k<-24.0f)
		v2_k=-24.0f;
	
	u1_k= v1_k/48.0f+0.5f;
	u2_k= v2_k/48.0f+0.5f;
	
	//==== Décalage entrée sortie
	err1_a=err1;
	err2_a=err2;
	
	v1_k_a=v1_k;
	v2_k_a=v2_k;

	// Limitation sortie
	if (u1_k >0.9f)
		u1_k=0.9f;
	else if (u1_k<0.1f)
		u1_k=0.1f;

	if (u2_k >0.9f)
		u2_k=0.9f;
	else if (u2_k<0.1f)
		u2_k=0.1f;
	
	PWM_MOTEUR1 = (2.0f* PWM50*(u1_k));// conversion en %
	PWM_MOTEUR2 = (2.0f* PWM50*(u2_k));// conversion en %
	
	TIM_SetCompare1( TIM1,  PWM_MOTEUR1);// ecriture de CCR1 de 16 bits
	TIM_SetCompare2( TIM1,  PWM_MOTEUR2);// ecriture de CCR2 de 16 bits	

	STM_EVAL_LEDOff(LED6);
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

void Trait_consigne(void){
	Consigne1=(k1*Anglemesure+k2*VitesseAngle)/0.10435f ;
	Consigne2=(k1*Anglemesure+k2*VitesseAngle)/0.10435f ;
	//à ajouter gestion de guidon
};

unsigned char * bintoascii(uint16_t data_bin)
{
	*(chain_ascii + 5)     = 0;
	*(chain_ascii + 4) = 0x30 + data_bin %10 ;// unité
	data_bin /= 10;
  *(chain_ascii + 3) = 0x30 + data_bin %10 ;// dizaine
	data_bin /= 10;
  *(chain_ascii + 2) = 0x30 + data_bin %10 ;// centaine
	data_bin /= 10;
  *(chain_ascii + 1) = 0x30 + data_bin %10 ;// unité de 1000
	data_bin /= 10;
  *(chain_ascii)     = 0x30 + data_bin %10 ;// 10 aine de 1000
	return chain_ascii;
	
}


/************************ (C) COPYRIGHT INSA TOULOUSE *****END OF FILE****/
