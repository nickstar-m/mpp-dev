#ifndef AD9850_STM32F0XX_H_
#define AD9850_STM32F0XX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"
#include <stdint.h>
#include "..\ad9850.h"

typedef struct
{
  GPIO_TypeDef *gpio;
  uint16_t pinmask;
} AD9850_STM32F0xx_Pin;

typedef struct
{
  AD9850_STM32F0xx_Pin pins[AD9850_PINS_AMOUNT];
} AD9850_STM32F0xx_Pinout;

typedef struct
{
  AD9850_GPIO_Interface interface;
  AD9850_STM32F0xx_Pinout pinout;
  AD9850_AssertFn assert_failure_handler;
} AD9850_STM32F0xx_GPIO_Driver;

extern const AD9850_GPIO_Interface AD9850_STM32F0XX_PINDRIVER_INTERFACE;

#ifdef __cplusplus
}
#endif

#endif /* AD9850_STM32F0XX_H_ */
