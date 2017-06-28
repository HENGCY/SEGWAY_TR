/**
  ******************************************************************************
  * @file    stm32f30x_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_it.h"
#include "main.h"
#include "fonctions.h"

/** @addtogroup STM32F3-Discovery_Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t UserButtonPressed;
extern __IO uint8_t DataReady;
extern __IO uint16_t Acq_ADC1[6];//Valeur des IN6, 7,8,9, 1 & 6 de ADC1
__IO uint32_t VAR_ADC12_IN6 , VAR_ADC12_IN7 , VAR_ADC12_IN8 , VAR_ADC12_IN9,VAR_ADC1_IN2,VAR_ADC1_IN5 ;// variables travail ADC moyennees de Acq_ADC1[6]
//extern uint8_t ST [10]; //test RX serie
extern uint8_t RX_USART [7];							//	Buffer reception
int32_t Consigne =0;
#define NB_MOY 8 // moyenne glissante 
__IO uint32_t i =0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f30x.s).                                            */
/******************************************************************************/
/**
  * @brief  This function handles EXTI1_IRQ Handler (Interruption for Gyro-Acc)
  * @param  None
  * @retval None
  */

extern bool ConfigOK;

void EXTI1_IRQHandler(void)
{ 
  STM_EVAL_LEDOn(LED10);	

	//if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	  Trait_Gyro_Acc ();
	  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)  == RESET);
	EXTI_ClearITPendingBit(EXTI_Line1);
}

/**
  * @brief  La fonction interruption pour DMA1_Channel1_IRQHandler (ADC et boucle de courant)
  * @param  None
  * @retval None
  */

void DMA1_Channel1_IRQHandler (void)
{
	static uint16_t i = 0;	

	if (DMA_GetITStatus(DMA1_IT_TC1) != RESET) {	
		i++;
		STM_EVAL_LEDOn(LED6); // boucle de courant 

		/* moyenne glissante NB_MOY passage*/
		VAR_ADC12_IN6 += Acq_ADC1[0];
		VAR_ADC12_IN7 += Acq_ADC1[1];
		VAR_ADC12_IN8 += Acq_ADC1[2];
		VAR_ADC12_IN9 += Acq_ADC1[3];
		VAR_ADC1_IN2  += Acq_ADC1[4];
		VAR_ADC1_IN5  += Acq_ADC1[5];


		if (i == NB_MOY){
			i =0;
			MOY_ADC12_IN6 = VAR_ADC12_IN6/NB_MOY;
			MOY_ADC12_IN7 = VAR_ADC12_IN7/NB_MOY;
			MOY_ADC12_IN8 = VAR_ADC12_IN8/NB_MOY;
			MOY_ADC12_IN9 = VAR_ADC12_IN9/NB_MOY;
			MOY_ADC1_IN2  = VAR_ADC1_IN2/NB_MOY;
			MOY_ADC1_IN5  = VAR_ADC1_IN5/NB_MOY;

			VAR_ADC12_IN6 = 0;
			VAR_ADC12_IN7 = 0;
			VAR_ADC12_IN8 = 0;
			VAR_ADC12_IN9 = 0;
			VAR_ADC1_IN2  = 0;
			VAR_ADC1_IN5  = 0;
				
			STM_EVAL_LEDToggle(LED9);
				
			if (ConfigOK){ 								//	une fois la configuration du système est fait
				//Trait_b_courant();					//	execution boucle de courant une fois ADC a trouvé la valeur moyenne de courant
			}
		}
	DMA_ClearITPendingBit(DMA1_IT_TC1);			
	}	
}


//USART2

void USART2_IRQHandler  (void)
{
  static int rx_index = 0;
	uint8_t tampon = 0; // varaible temporaire pour tester la premier caratere de trame 
	STM_EVAL_LEDToggle(LED4);
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // Received characters modify string
  { 
		tampon = USART_ReceiveData(USART2);
		if (tampon == '<'){
			rx_index = 1;
			RX_USART[0] = '<';
		} else{
			RX_USART[rx_index++] = tampon;		
		}	
			
    if (rx_index > (sizeof(RX_USART) - 1)){
			rx_index = 0;
			if (RX_USART[0] == '<'&& RX_USART[6] == 'b') // trame complet
			{
				if ((RX_USART[2]<58) && (RX_USART[2]>47)&&(RX_USART[3]<58) && (RX_USART[3]>47)&&(RX_USART[4]<58) && (RX_USART[4]>47)&&(RX_USART[5]<58) && (RX_USART[5]>47))
				{
					// contenu est tous bien corespondant
					Consigne =((RX_USART[2]-48)*1000 + (RX_USART[3]-48)*100 + (RX_USART[4]-48)*10 + (RX_USART[5]-48));
					switch (RX_USART[1]){
						//case de consigne de couple positive
						case 'c' : Consigne1=(float)Consigne/100.0f; 
											 break;
						//case de consigne de couple negative
						case 'd' : Consigne1=(float)Consigne/-100.0f;   
											 break;
						default : Consigne1=0.0f;   
					}
				}else {
					Consigne1=0.0f;
				}
			}
			else {Consigne1=0.0f;}
		}
	}		
}

void TIM2_IRQHandler  (void)
{				
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {   
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			  STM_EVAL_LEDToggle(LED8);
				//Trait_consigne();
    }
		

}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

