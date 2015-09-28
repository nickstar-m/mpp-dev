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
  int32_t Temperature;
	static int32_t Calc = 0;
  int32_t Avg_Slope;

	Avg_Slope = (Ts_Cal2 - Ts_Cal1) / 8;
  
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  Temperature = ADC_GetConversionValue(ADC1); 
  Calc += ((((Temperature)*21/20 - (int32_t)Ts_Cal1) * 100 / Avg_Slope) + 300) - Calc / 8;

	return Calc / 80;
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
  	
		TerminalBuffer[size] = 0;     //Zero ended string
  	USART_PutStr(TerminalBuffer); // Echo
		return TerminalBuffer;
	}
	return ""; // No data in the Buffer;
}

void TerminalProcessor(void)
{
	uint8_t pointer = 0;
  uint8_t state = 0;
	uint8_t command;
	uint8_t variable;
	
	// 0 start
	// 1 command without param
	// 2 command with param
	// 3 first param (variable)
	// 4 last param (variable or valuee)
	// 7 end
	// 8 illegal command
  // 9 illegal parameter/variable
	// 10 illegal value
  
	//-----------------------------
	// Command parser
	//-----------------------------
	while ((state < 7) || !TerminalBuffer[pointer])
	{
	  switch (state)
		{
		  case 0:
				if (TerminalBuffer[pointer] == 0x0d || TerminalBuffer[pointer] == 0x0a)
				{
				  state = 7;
				}
				else if (TerminalBuffer[pointer] >= 0x41 && TerminalBuffer[pointer] >= 0x5a)
        {
				  state = 1;
				}
				else
				{
				  state = 8;
				}	
				break;
		  case 1:
				switch(buffer[pointer])
        {
				  case 'G':
					  pointer++;	
					  switch(buffer[pointer])
            {
    				  case 'E':
    					  pointer++;	
      					  switch(buffer[pointer])
                  {
    				        case 'T':
        				      if (buffer[++pointer] == ' ')
											{
												state = 2;
												while (buffer[++pointer] != ' ');
												sprintf(buffer, "GET command");
												state = 8;
											}	
											else
											{
												state = 8;
											}
										  break;
          					default:
		          				state = 8; 
				    	        break;
									}	
    					default:
		    				state = 8; 
				    	  break;	
						}	
  				  break;
				  case 'S':
  				  break;
				  case 'L':
  				  break;
				  case 'V':
  				  break;
					default:
						state = 8; 
					  break;	
				}
				break;
		  case 2:
				break;
		  case 7:
				break;
		  case 8:
				sprintf(buffer, "Illegal command");
				break;
		  case 9:
				sprintf(buffer, "Unknown variable");
				break;
		  case 10:
				sprintf(buffer, "Invalid value");
				break;
		}
	}	
}
