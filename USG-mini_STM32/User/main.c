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
  char TX_buffer[128];
  char RX_buffer[64];
#endif


// Additional global variables
__IO uint16_t delay_count = 0;

// ---------------------------------------------------------------------------
// Interrupt handlers
// ---------------------------------------------------------------------------
void SysTick_Handler (void)
{
  if (delay_count) delay_count--;
}
	
// ---------------------------------------------------------------------------
// Utilites
// ---------------------------------------------------------------------------
void Delay_ms (uint16_t delay_ms)
{
	delay_count = delay_ms;
	while (delay_count);
}

void GetUptime(RTC_TimeTypeDef uptime)
{
  RTC_GetTime(RTC_Format_BIN, &uptime);
}

void GetUptimeStr(char *uptime_str)
{
  RTC_TimeTypeDef uptime; // Uptime
  RTC_GetTime(RTC_Format_BIN, &uptime);
	sprintf(uptime_str,"%02d:%02d:%02d",uptime.RTC_Hours, uptime.RTC_Minutes, uptime.RTC_Seconds);
}

// ---------------------------------------------------------------------------
// Main()
// ---------------------------------------------------------------------------
int main(void){
  int i;
	char c[8];
	uint16_t adc, temperature;
	
	// ---------------------------------------------------------------------------
  // Init section
  // ---------------------------------------------------------------------------
	GPIO_init();
	USART_init_();
	ADC_init ();
	RTC_init();
  
	SysTick_Config(SystemCoreClock / 1000); // 1 ms
	
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
		GetUptimeStr(c);
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
		ADC_StartOfConversion(ADC1);
		for (i=0; i<8; i++){
  		USART_SendData(USART1, c[i]);
  		Delay_ms(10);		
		}
		USART_SendData(USART1, 13);
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);

    // Delay ~0.2 sec.
    Delay_ms(200);
   
    //(#) Get the voltage values, using ADC_GetConversionValue() function
    adc = ADC_GetConversionValue(ADC1);
    // Calculate corresponding voltage level    
    // V25 = (4095 * 1.43V) / 3.6V = 1627
    // Avg_Slope = 4.3mV / (3.6V / 4095) = 4.892 ~ 5
    // Output the values
	  temperature = ((1952 - adc) / 6) + 25;

 		USART_SendData(USART1, temperature/10 + 48);
    Delay_ms(10);
		USART_SendData(USART1, temperature%10 + 48);
		Delay_ms(10);
		USART_SendData(USART1, 176);
		Delay_ms(10);
		USART_SendData(USART1, 67);
		Delay_ms(10);
		USART_SendData(USART1, 13);
		Delay_ms(3000);
	}
}
