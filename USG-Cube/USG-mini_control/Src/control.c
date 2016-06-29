#include "control.h"
#include "main.h"

// Display functions
//====================

// Display refresh
void DisplayRefresh(void)
{
  static uint8_t CurrentDigit = 0;
	const uint16_t BlinkPattern = 0xff00; // 1 sec pattern
	
	HAL_GPIO_WritePin(DigitPinsPort[CurrentDigit], DigitPins[CurrentDigit], GPIO_PIN_RESET);
	CurrentDigit =  HAL_GetTick() % 8;
  for (unsigned i=0; i<8; ++i)
	{
  	if (Display_ON)
		  HAL_GPIO_WritePin(SegmentPinsPort[i], SegmentPins[i], (indicator[CurrentDigit] & (1<<i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		else
		  HAL_GPIO_WritePin(SegmentPinsPort[i], SegmentPins[i], GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(DigitPinsPort[CurrentDigit], DigitPins[CurrentDigit], GPIO_PIN_SET);
}

void DisplayPut(uint8_t ind, const char *s, uint8_t attr)
{
  unsigned pointer = 0;
	for (unsigned i=0; (i<6) && (pointer<3) && (s[pointer]!=0); ++i)
	{
		if ((s[i] == 0x2e) && (pointer > 0))
		{
		  indicator[pointer + (ind == INDICATOR_LEFT ? 0 : 3) - 1] |= 0x80;
		}
		else
		{
		  indicator[pointer + (ind == INDICATOR_LEFT ? 0 : 3)] = Font[s[i] - 0x20];
			++pointer;
		}	
	}	
}	

void DisplayOn(void)
{
  Display_ON = 1;
}	

void DisplayOff(void)
{
  Display_ON = 0;
}	

void LED_Control(uint8_t led, uint8_t state)
{
  switch (state)
	{	
		case LED_STATE_OFF :
			indicator[LED_BYTE] &= ~(1<<led); 
			break;
		case LED_STATE_ON :
			indicator[LED_BYTE] |= 1<<led;
		  indicator[BLINK_REG] &= ~(1<<(led == LED_LEFT ? BLINK_LED_LEFT : BLINK_LED_RIGHT));
			break;
		case LED_STATE_BLINK :
			indicator[LED_BYTE] |= 1<<led; 
		  indicator[BLINK_REG] |= 1<<(led == LED_LEFT ? BLINK_LED_LEFT : BLINK_LED_RIGHT);
			break;
	}	
}

// Keyboard functions
//====================


// Digital thermometer functions
//====================


// Beeper
//====================
void Beep(uint8_t beeps_count, uint16_t beep_duration, uint16_t pause_duration)
{
  for (unsigned i=0; i<beeps_count; ++i)
	{
	  HAL_GPIO_WritePin(SPK_GPIO_Port, SPK_Pin, GPIO_PIN_SET);
	  HAL_Delay(beep_duration);
	  HAL_GPIO_WritePin(SPK_GPIO_Port, SPK_Pin, GPIO_PIN_RESET);

		if (i<(beeps_count-1)) HAL_Delay(pause_duration);
	}
}

// I2C interface functions
//====================
