/******************************************************************************
 * @file     utilites.c
 * @brief    USG-mini generator main board firmware
 * @version  V1.00
 * @date     23. September 2015
 * @author   Nickstar
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2015 MedPromPrylad

   ---------------------------------------------------------------------------*/
# include "utilites.h"

#define Ts_Cal1    (*((uint16_t*)0x1FFFF7B8))
#define Ts_Cal2    (*((uint16_t*)0x1FFFF7C2))

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

int32_t GetTemperature(void)
{
  int32_t Temperature = 0;
	int32_t Calc;
  int32_t Avg_Slope;

	Avg_Slope = (Ts_Cal2 - Ts_Cal1) << 3;
  
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  Temperature = ADC_GetConversionValue(ADC1); 
  Calc = ((((int32_t)Ts_Cal1 - (Temperature * 6 / 5)) * 10 / Avg_Slope) + 300) / 10;

	return Calc;
}

void USART_PutStr(char* str)
{

}

char* USART_GetStr(void)
{
  return "";
}
