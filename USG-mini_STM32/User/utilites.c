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

void GetUptime(RTC_TimeTypeDef *uptime)
{
	RTC_GetTime(RTC_Format_BIN, uptime);
}

void GetTemperature(int16_t *temperature)
{
	int16_t tmp;
	static int16_t Calc = 2000;
  static int16_t Avg_Slope = 0;

	if (!Avg_Slope) Avg_Slope = (Ts_Cal2 - Ts_Cal1) / 8;
  
  ADC_StartOfConversion(ADC1);
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET){};
	
	tmp = ADC_GetConversionValue(ADC1);
	
  sprintf(buffer, "calc=%d\rAvg_Slope=%d\rtmp=%d\r\r", Calc, Avg_Slope, tmp);
	USART_PutStr(buffer);
		
	Calc += (((tmp*21/20 - Ts_Cal1) * 100 / Avg_Slope) + 300) - Calc / 8;

	*temperature = Calc / 8;
}

int USART_PutStr(char* str)
{
  int i;
	if (TX_Buffer.PushIndex + strlen(str) > sizeof(TX_Buffer.Buffer)) {
	  return -1;
	}
	else {
		memcpy(TX_Buffer.Buffer, str, strlen(str));
		TX_Buffer.PushIndex = 0;
		for (i=0; i < strlen(str); i++){
  		USART_SendData(USART1, TX_Buffer.Buffer[TX_Buffer.PushIndex++]);
			TX_Buffer.PushIndex %= sizeof(TX_Buffer.Buffer);
			//TX_Buffer.Size++;
			Delay_ms(5);		
		}
	return strlen(str);
	}
}

char* USART_GetStr(void)
{
	uint8_t size;
	size = RX_Buffer.Size;
	if (size > 0){
	  if (size > sizeof(RX_Buffer.Buffer) - RX_Buffer.PopIndex)
		{
		  memcpy(TerminalBuffer, &RX_Buffer.Buffer[RX_Buffer.PopIndex], sizeof(RX_Buffer.Buffer) - RX_Buffer.PopIndex );
		  memcpy(&TerminalBuffer[sizeof(RX_Buffer.Buffer) - RX_Buffer.PopIndex], RX_Buffer.Buffer, size + RX_Buffer.PopIndex - sizeof(RX_Buffer.Buffer));
  		RX_Buffer.PopIndex += size - sizeof(RX_Buffer.Buffer);
		}
		else
		{	
		  memcpy(TerminalBuffer, &RX_Buffer.Buffer[RX_Buffer.PopIndex], size);
			RX_Buffer.PopIndex += size;
		}
		RX_Buffer.Size -= size;
  	
		TerminalBuffer[size] = 0;     // Null terminated string
  	USART_PutStr(TerminalBuffer); // Echo
		return TerminalBuffer;
	}
	return NULL; // No data in the Buffer;
}

void Monitor(void)
{
	GetTemperature(&DevState.T_CASE);	  /*!< Temperature of the case (STM32 internal sensor) */
  DevState.T_KEYS = 250;							/*!< Temperature of the transistors (external sensor) */
  DevState.VIN = 2200;    						/*!< Electrical grid RMS voltage */
  DevState.V33 = 330;									/*!< 3.3V supply voltage */
  DevState.V15 = 1500;								/*!< 5V supply voltage */
  DevState.HV = 3110;									/*!< The supply voltage of the keys */
  DevState.CG = 220;									/*!< Current of the generator */
  DevState.CH = 300;									/*!< Current of the heater */
  GetUptime(&DevState.UPTIME);       	/*!< Uptime aftr last power on */
  DevState.HEAT_STATE = 0;						/*!< Heater state (ON/OFF) */
  DevState.TRIAC_ANGLE = 12;					/*!< Triac control algle, degrees */
  DevState.GEN_STATE = 1;		  				/*!< Generator state (ON/OFF)*/
  DevState.LAST_ERROR = 0;						/*!< Last error code */
}
