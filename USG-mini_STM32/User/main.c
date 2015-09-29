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
	DMA_USART_TX_init();
	
	ADC_init ();
	
	RTC_init();
	
	NVIC_init();
	
	SysTick_Config(SystemCoreClock / 1000); // 1 ms
	
	InitEnvironment();
	
	//(#) Enable the internal connection of Temperature sensor, Vrefint or Vbat
  //    sources with the ADC channels using ADC_TempSensorCmd(), ADC_VrefintCmd()
  //    or ADC_VbatCmd() functions.
  ADC_TempSensorCmd(ENABLE);

  //(#) select the ADC_Channel_16(Temperature sensor), ADC_Channel_17(Vrefint)
  //    or ADC_Channel_18(Voltage battery) using ADC_ChannelConfig() function
  ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor, ADC_SampleTime_28_5Cycles);

  // Start the first conversion
  ADC_StartOfConversion(ADC1);
	//DMA_USART_TX_init();
  
	// Main loop
	while(1){
		//GetUptimeStr(c);
		//GPIO_SetBits(GPIOA, GPIO_Pin_0);
		//ADC_StartOfConversion(ADC1);
		
		//while (!USART_PutStr(c));
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
 		//sprintf(buffer, "\n\r");
		//while (!USART_PutStr(buffer)){};

    // Delay ~0.2 sec.
    //Delay_ms(200);
   
    //Get temperature
		//temperature = GetTemperature();

 		//sprintf(buffer, "%d�C\n\r", temperature);
		//while (!USART_PutStr(buffer));
		;
		if (USART_GetStr() != NULL)
		{	
		  TerminalProcessor();
		}	
		Delay_ms(10);
	}
}
