/**************************************************************************//**
 * @file     main.c
 * @brief    USG-mini generator main board firmware
 * @version  V1.00
 * @date     21. September 2015
 * @author   Nickstar
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2015 MedPromPrylad

   ---------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------
// Basic definitions
// ---------------------------------------------------------------------------
#define  INTERFACE_TYPE   TERMINAL
                          //TERMINAL
													//I2C 
													//NONE

// ---------------------------------------------------------------------------
// Includes
// ---------------------------------------------------------------------------
#include "main.h"

// ---------------------------------------------------------------------------
// Global variables
// ---------------------------------------------------------------------------
#if (INTERFACE_TYPE == TERMINAL)
  USART_BufferTypeDef RX_Buffer = {"", 0, 0};
  USART_BufferTypeDef TX_Buffer = {"", 0, 0};
	char buffer[128];
	char TerminalBuffer[128];
#endif

const DevConstantsTypeDef DevConstants = CONST_INIT_DATA;
DevStateTypeDef DevState;
DevStatisticsTypeDef DevStatistics;
DevControlVariablesTypeDef DevControlVariables;
DevAdditionalControlVariablesTypeDef DevAdditionalControlVariables;

// Additional global variables
volatile uint16_t delay_count = 0;

// ---------------------------------------------------------------------------
// Init Device Environment
// ---------------------------------------------------------------------------
void InitEnvironment(void)
{	
  // Start the first conversion
  ADC_StartOfConversion(ADC1);
  
	Monitor();
	Delay_ms(500);
}
	
// ---------------------------------------------------------------------------
// Main()
// ---------------------------------------------------------------------------
int main(void){
	// ---------------------------------------------------------------------------
  // Init section
  // ---------------------------------------------------------------------------
	GPIO_init();
	
	USART_init_();
	DMA_USART_TX_init();
	DMA_USART_RX_init();
	
	ADC_init ();
	
	RTC_init();
	
	NVIC_init();
	
	SysTick_Config(SystemCoreClock / 1000); // 1 ms
	
	//DMA_USART_TX_init();
  
	InitEnvironment();
	
	// Main loop
	while(1){
		if (USART_GetStr() != NULL)
		{	
		  TerminalProcessor();
		}	
		Delay_ms(100);
		Monitor();
	  GPIO_SetBits(GPIOA, GPIO_Pin_0);
  	Delay_ms(100);
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);  
	}
}
