#include "stm32f0xx_hal.h"
#include <stdint.h>
#include <stdlib.h>
#include "ad9850_stm32f0xx.h"


#ifndef NDEBUG
  #define AD9850_STM32F0XX_ASSERT(x) \
    { \
      if (!(x)) \
      { \
        AD9850_STM32F0xx_GPIO_Driver *driver = ((AD9850_STM32F0xx_GPIO_Driver*)interface); \
        if (driver->assert_failure_handler != NULL) \
          driver->assert_failure_handler(__FILE__, __LINE__); \
      } \
    }

  #define AD9850_STM32F0XX_RETURN_ASSERT(x,ret) \
    do { \
      int condition = (x); \
      AD9850_STM32F0XX_ASSERT(condition) \
      if (!condition) \
        return (ret); \
    } while (0)
#else
  #define AD9850_STM32F0XX_ASSERT(x)
  #define AD9850_STM32F0XX_RETURN_ASSERT(x,ret)
#endif


static AD9850_Result stm32f0xx_default_pin_configure(
  AD9850_GPIO_Interface *interface, AD9850_Pin pin, AD9850_PinMode mode)
{
  AD9850_STM32F0XX_RETURN_ASSERT(interface != NULL, AD9850_RESULT_ERROR);

  AD9850_STM32F0xx_GPIO_Driver *driver = (AD9850_STM32F0xx_GPIO_Driver*)interface;
  AD9850_STM32F0xx_Pin *hw_pin = &driver->pinout.pins[pin];

  AD9850_STM32F0XX_RETURN_ASSERT(hw_pin != NULL, AD9850_RESULT_ERROR);
  AD9850_STM32F0XX_RETURN_ASSERT(hw_pin->gpio != NULL, AD9850_RESULT_ERROR);

	GPIO_InitTypeDef gpio_config;

  switch (mode)
  {
    case AD9850_PINMODE_OUTPUT:
      gpio_config.Mode = GPIO_MODE_OUTPUT_PP;
		  gpio_config.Pull = GPIO_NOPULL;
      break;

    case AD9850_PINMODE_INPUT:
      //gpio_config.GPIO_Mode = GPIO_Mode_IN_FLOATING;
      gpio_config.Mode = GPIO_MODE_INPUT;
		  gpio_config.Pull = GPIO_NOPULL;
      break;

    default:
      AD9850_STM32F0XX_ASSERT(0);
      break;
  }

  gpio_config.Speed = GPIO_SPEED_HIGH;
	gpio_config.Pin = hw_pin->pinmask;

  if (hw_pin->gpio != NULL)
    HAL_GPIO_Init(hw_pin->gpio, &gpio_config);

  return AD9850_RESULT_OK;
}

static AD9850_Result stm32f0xx_default_pin_write(
  AD9850_GPIO_Interface *interface, AD9850_Pin pin, AD9850_PinState value)
{
  AD9850_STM32F0XX_RETURN_ASSERT(interface != NULL, AD9850_RESULT_ERROR);

  AD9850_STM32F0xx_GPIO_Driver *driver = (AD9850_STM32F0xx_GPIO_Driver*)interface;
  AD9850_STM32F0xx_Pin *hw_pin = &driver->pinout.pins[pin];

  AD9850_STM32F0XX_RETURN_ASSERT(hw_pin != NULL, AD9850_RESULT_ERROR);
  AD9850_STM32F0XX_RETURN_ASSERT(hw_pin->gpio != NULL, AD9850_RESULT_ERROR);

  HAL_GPIO_WritePin(hw_pin->gpio, hw_pin->pinmask,
    (value == AD9850_PINSTATE_LOW ? GPIO_PIN_RESET : GPIO_PIN_SET));


  return AD9850_RESULT_OK;
}

static AD9850_Result stm32f0xx_default_pin_read(
  AD9850_GPIO_Interface *interface, AD9850_Pin pin, AD9850_PinState *value)
{
  AD9850_STM32F0XX_RETURN_ASSERT(interface != NULL, AD9850_RESULT_ERROR);
  AD9850_STM32F0XX_RETURN_ASSERT(value != NULL, AD9850_RESULT_ERROR);

  AD9850_STM32F0xx_GPIO_Driver *driver = (AD9850_STM32F0xx_GPIO_Driver*)interface;
  AD9850_STM32F0xx_Pin *hw_pin = &driver->pinout.pins[pin];

  AD9850_STM32F0XX_RETURN_ASSERT(hw_pin != NULL, AD9850_RESULT_ERROR);
  AD9850_STM32F0XX_RETURN_ASSERT(hw_pin->gpio != NULL, AD9850_RESULT_ERROR);

  uint8_t out_bit = HAL_GPIO_ReadPin(hw_pin->gpio, hw_pin->pinmask);
  *value = (out_bit == GPIO_PIN_RESET ? AD9850_PINSTATE_LOW : AD9850_PINSTATE_HIGH);
	
  return AD9850_RESULT_OK;
}

const AD9850_GPIO_Interface AD9850_STM32F0XX_PINDRIVER_INTERFACE =
{
  stm32f0xx_default_pin_configure,
  stm32f0xx_default_pin_write,
  stm32f0xx_default_pin_read
};
