#include "stm32f0xx_hal.h"
#include "onewire.h"

#define INDICATOR_LEFT  0
#define INDICATOR_RIGHT 1

#define LED_LEFT  3
#define LED_RIGHT 2
#define LED_STATE_OFF   0x00
#define LED_STATE_ON    0x01
#define LED_STATE_BLINK 0x02

void DisplayOn(void);
void DisplayOff(void);
void DisplayRefresh(void);
void Beep(uint8_t beeps_count, uint16_t beep_duration, uint16_t pause_duration);
void DisplayPut(uint8_t ind, const char *s, uint8_t attr);
void LED_Control(uint8_t led, uint8_t state);

