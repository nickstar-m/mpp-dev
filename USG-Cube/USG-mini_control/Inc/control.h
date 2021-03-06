#include "stm32f0xx_hal.h"

#ifndef __CONTROL_H
#define __CONTROL_H

#define LED_BYTE  0x06

const uint16_t DigitPins[]   = {DIG1_Pin, DIG2_Pin, DIG3_Pin, DIG4_Pin, DIG5_Pin, DIG6_Pin, IND1_Pin, IND2_Pin};
GPIO_TypeDef* DigitPinsPort[]   = {DIG1_GPIO_Port, DIG2_GPIO_Port, DIG3_GPIO_Port, DIG4_GPIO_Port, DIG5_GPIO_Port, DIG6_GPIO_Port, IND1_GPIO_Port, IND2_GPIO_Port};
const uint16_t SegmentPins[] = {SEG_a_Pin, SEG_b_Pin, SEG_c_Pin, SEG_d_Pin, SEG_e_Pin, SEG_f_Pin, SEG_g_Pin, DP_Pin};
GPIO_TypeDef* SegmentPinsPort[] = {SEG_a_GPIO_Port, SEG_b_GPIO_Port, SEG_c_GPIO_Port, SEG_d_GPIO_Port, SEG_e_GPIO_Port, SEG_f_GPIO_Port, SEG_g_GPIO_Port, DP_GPIO_Port};

char const Font[] = {
	0x00, 0x30, 0x22, 0x63, 0x6d, 0x2d, 0x7e, 0x02, 0x39, 0x0f, 0x76, 0x46, 0x80, 0x40, 0x80, 0x52,  //20-2F
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x09, 0x89, 0x61, 0x41, 0x43, 0x53,  //30-3F
  0x5f, 0x77, 0x7f, 0x39, 0x3f, 0x79, 0x71, 0x3d, 0x76, 0x06, 0x1e, 0x76, 0x38, 0x37, 0x76, 0x3f,  //40-3F
  0x73, 0xfe, 0x77, 0x6d, 0x31, 0x3e, 0x3e, 0x3e, 0x76, 0x6e, 0x5b, 0x39, 0x64, 0x0f, 0x23, 0x08,  //50-3F
  0x20, 0x5f, 0x7c, 0x58, 0x5e, 0x7b, 0x71, 0x6f, 0x74, 0x04, 0x0c, 0x78, 0x30, 0x54, 0x54, 0x5c,  //60-3F
  0x73, 0x67, 0x50, 0x6d, 0x78, 0x1c, 0x1c, 0x1c, 0x76, 0x6e, 0x5b, 0x79, 0x06, 0x4f, 0x40, 0x49,  //70-3F
};

uint8_t Display_ON = 0;
#define BLINK_REG 0x07
#define BLINK_IND0      0
#define BLINK_IND1      1
#define BLINK_POWER_BAR 2
#define BLINK_LED_LEFT  3
#define BLINK_LED_RIGHT 4

char indicator[BLINK_REG + 1]; // display buffer

void DisplayOn(void);
void DisplayOff(void);
void DisplayRefresh(void);
void Beep(uint8_t beeps_count, uint16_t beep_duration, uint16_t pause_duration);

typedef enum {ALIGHT, BLINK} IndicatorAttribute;

#endif
