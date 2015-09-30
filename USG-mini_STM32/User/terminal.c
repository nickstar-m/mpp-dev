/******************************************************************************
 * @file     terminal.c
 * @brief    USG-mini generator main board firmware
 * @version  V1.00
 * @date     29. September 2015
 * @author   Nickstar
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2015 MedPromPrylad

   ---------------------------------------------------------------------------*/
	 
#include "terminal.h"
#include "utilites.h"

 void TerminalProcessor(void)
{
	uint8_t pointer = 0;
	uint8_t token_length = 0;
	uint8_t command_index = 0;
	uint8_t variable_index = 0;
  uint8_t state = 0;
	
	// 0 start
	// 2 command token
	// 3 param @1
	// 4 separator
	// 5 param @2

	// 8 ended successfully
	// 9 illegal command
  // 10 illegal parameter
	// 11 illegal parameter type
	// 12 incorrect syntax
	// 13 value is less than allowed
	// 14 value is larger than allowed
  
	//-----------------------------
	// Command parser
	//-----------------------------
	while ((state < 8) && TerminalBuffer[pointer])
	{
	  switch (state)
		{
		  case 0: // Check of the command presence
				if (TerminalBuffer[pointer] == '\r' || TerminalBuffer[pointer] == '\n') // CR & LF // No command
				{
   		  	sprintf(buffer, PROMT);
	        USART_PutStr(buffer);
					return;
				}	
				else
				  state = 2;
				break;
		  case 2: // Parse command token
				if (TerminalBuffer[pointer] == ' ') while (TerminalBuffer[++pointer] == ' '); // skip spaces if needed
			  while (TerminalBuffer[pointer + ++token_length] != ' ' &&
      				 TerminalBuffer[pointer + token_length] != '\r' &&
               TerminalBuffer[pointer + token_length] != '\n'); //detect the token length  
			  state = 9;
			  while (command_index < NUMBER_OF_COMMANDS)
				{
				  if (!strncmp(&TerminalBuffer[pointer], (*CmdPtr)[command_index], token_length))
          { 
					  if (strlen((*CmdPtr)[command_index]) == token_length)
							state = 7;
            else if (strstr((*CmdPtr)[command_index], "@1") != NULL)
						  state = 3;
						else
						  state = 9;
						break;
				  }
					command_index++;
				}
 			  break;
		  case 3: //Parse parameter @1
				state = 8;
				break;
		  case 4: //Parse separator
				state = 8;
				break;
		  case 5: //Parse parameter @2
				state = 8;
				break;
		  default:
				state = 8;
			  break;
		}
	}	
	if (state == 8)
  {
	  switch (command_index)
		{
		  case 2:
  			Status();
			  break;
		  case 4:
  			Ver();
			  break;
		  case 5:
				Ser();
		  break;
			default:	
  			sprintf(buffer, "Entered the command \"%s\"!", (*CmdPtr)[command_index]);
    		USART_PutStr(buffer);
			  break;
		}
	}
	else if (state > 8)
	{
	  sprintf(buffer, (*ErrPtr)[state-9], "@1", "@2");
  	USART_PutStr(buffer);
	}	
	sprintf(buffer, PROMT);
	USART_PutStr(buffer);

}

void Ver(void)
{
  sprintf(buffer, "Model number:     %s\r\n", DevConstants.MODEL);
	USART_PutStr(buffer);
  sprintf(buffer, "Hardware version: %s\r\n", DevConstants.REV_HW);
	USART_PutStr(buffer);
  sprintf(buffer, "Software version: %s", DevConstants.REV_FW);
	USART_PutStr(buffer);
}

void Ser(void)
{
  sprintf(buffer, "Model number:              %s\r\n", DevConstants.MODEL);
	USART_PutStr(buffer);
  sprintf(buffer, "Production date:           %s\r\n", DevConstants.PROD_DATE);
	USART_PutStr(buffer);
  sprintf(buffer, "Serial number:             %s\r\n\r\n", DevConstants.SERIAL);
	USART_PutStr(buffer);
  sprintf(buffer, "End product model:         %s\r\n", DevConstants.PMODEL);
	USART_PutStr(buffer);
  sprintf(buffer, "End product serial number: %s", DevConstants.PSERIAL);
	USART_PutStr(buffer);
}

void Status(void)
{
  //DevState.HV = 3110;									/*!< The supply voltage of the keys */
  //DevState.CG = 220;									/*!< Current of the generator */
  //DevState.CH = 300;									/*!< Current of the heater */
  //GetUptime(DevState.UPTIME);       	/*!< Uptime aftr last power on */
  //DevState.HEAT_STATE = 0;						/*!< Heater state (ON/OFF) */
  //DevState.TRIAC_ANGLE = 12;					/*!< Triac control algle, degrees */
  //DevState.GEN_STATE = 1;		  				/*!< Generator state (ON/OFF)*/
  //DevState.LAST_ERROR = 0;						/*!< Last error code */
	
	sprintf(buffer, "Uptime:              %02d:%02d:%02d\r\n\r\n", DevState.UPTIME.RTC_Hours, DevState.UPTIME.RTC_Minutes, DevState.UPTIME.RTC_Seconds);
	USART_PutStr(buffer);
	sprintf(buffer, "CASE temperature:    %d.%01u°C\r\n", DevState.T_CASE / 10, DevState.T_CASE % 10);
	USART_PutStr(buffer);
  sprintf(buffer, "KEYS temperature:    %d.%01u°C\r\n\r\n", DevState.T_KEYS / 10, DevState.T_KEYS % 10);
	USART_PutStr(buffer);
  sprintf(buffer, "Grid RMS voltage:    %d.%01dV\r\n", DevState.VIN / 10, DevState.VIN % 10);
	USART_PutStr(buffer);
  sprintf(buffer, "3.3V supply voltage: %d.%02dV\r\n", DevState.V33 / 100, DevState.V33 % 100);
	USART_PutStr(buffer);
  sprintf(buffer, "15V supply voltage:  %d.%02dV", DevState.V15 / 100, DevState.V15 % 100);
	USART_PutStr(buffer);
}
