#include <stdlib.h>
#include <stdint.h>
#include "ad9850.h"


#ifndef NDEBUG
  #define AD9850_ASSERT(x) \
    { \
      if (!(x)) \
      { \
        if (dds->cfg.assert_failure_handler != NULL) \
          dds->cfg.assert_failure_handler(__FILE__, __LINE__); \
      } \
    }

  #define AD9850_RETURN_ASSERT(x,ret) \
    do { \
      int condition = (x); \
      AD9850_ASSERT(condition) \
      if (!condition) \
        return (ret); \
    } while (0)
#else
  #define AD9850_ASSERT(x)
  #define AD9850_RETURN_ASSERT(x,ret)
#endif

#define AD9850_RETURN_IF_ERROR(result) \
  do {\
    AD9850_Result r = (result); \
    if (r != AD9850_RESULT_OK) \
      return r; \
  } while (0)


#define AD9850_MASK_PHASE	    0xf800000000
#define AD9850_MASK_POWER	    0x0400000000
#define AD9850_MASK_CONTROL	  0x0300000000
#define AD9850_MASK_FREQUENCY	0x00ffffffff
	
AD9850_Result ad9850_config(AD9850 *dds);
AD9850_Result ad9850_init(AD9850 *dds, const AD9850_Config *config);
AD9850_Result ad9850_write_control_byte(AD9850 *dds, uint8_t value);
AD9850_Result ad9850_write_word(AD9850 *dds, uint64_t word);
AD9850_Result ad9850_reset(AD9850 *dds);
AD9850_Result ad9850_send(AD9850 *dds, uint64_t value, uint8_t bit_count);

AD9850_Result ad9850_init(AD9850 *dds, const AD9850_Config *config)
{
  AD9850_RETURN_ASSERT(dds != NULL, AD9850_RESULT_ERROR);
  AD9850_RETURN_ASSERT(config != NULL, AD9850_RESULT_ERROR);
  AD9850_RETURN_ASSERT(config->gpios != NULL, AD9850_RESULT_ERROR);
  AD9850_RETURN_ASSERT(config->gpios->write != NULL, AD9850_RESULT_ERROR);

  dds->cfg = *config;

  AD9850_GPIO_Interface *const gpios = dds->cfg.gpios;

  if (gpios->configure != NULL)
  {
    AD9850_RETURN_IF_ERROR(gpios->configure(gpios, AD9850_PIN_FQ_UD, AD9850_PINMODE_OUTPUT));
    AD9850_RETURN_IF_ERROR(gpios->configure(gpios, AD9850_PIN_W_CLK, AD9850_PINMODE_OUTPUT));
    AD9850_RETURN_IF_ERROR(gpios->configure(gpios, AD9850_PIN_RESET, AD9850_PINMODE_OUTPUT));
  }

  if (dds->cfg.mode == AD9850_MODE_SERIAL)
  {
    // dds->displayfunction = AD9850_FLAG_4BITMODE | AD9850_FLAG_1LINE | AD9850_FLAG_5x8DOTS;
    dds->dp_first = AD9850_PIN_D7;
    dds->dp_amount = 1;
  }
  else
  {
    // dds->displayfunction = AD9850_FLAG_8BITMODE | AD9850_FLAG_1LINE | AD9850_FLAG_5x8DOTS;
    dds->dp_first = AD9850_PIN_D0;
    dds->dp_amount = 8;
  }

  if (ad9850_config(dds) != AD9850_RESULT_OK)
    return AD9850_RESULT_ERROR;
	
	ad9850_reset(dds);

  /* Clear it off */
  // AD9850_RETURN_IF_ERROR(AD9850_clear(display));

  return AD9850_RESULT_OK;
}

AD9850_Result ad9850_reset(AD9850 *dds)
{
  AD9850_RETURN_ASSERT(dds != NULL, AD9850_RESULT_ERROR);
  AD9850_RETURN_ASSERT(dds->cfg.gpios != NULL, AD9850_RESULT_ERROR);
  AD9850_RETURN_ASSERT(dds->cfg.gpios->write != NULL, AD9850_RESULT_ERROR);
	
  AD9850_GPIO_Interface *const gpios = dds->cfg.gpios;

	AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_RESET, AD9850_PINSTATE_HIGH)); //do {}while(1);
	__nop();
	__nop();
  AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_RESET, AD9850_PINSTATE_LOW));

	return AD9850_RESULT_OK;
}

AD9850_Result ad9850_write_word(AD9850 *dds, uint64_t word)
{
  AD9850_RETURN_ASSERT(dds != NULL, AD9850_RESULT_ERROR);
  AD9850_RETURN_ASSERT(dds->cfg.gpios != NULL, AD9850_RESULT_ERROR);
  AD9850_RETURN_ASSERT(dds->cfg.gpios->write != NULL, AD9850_RESULT_ERROR);
	
	AD9850_RETURN_IF_ERROR(ad9850_send(dds, word, 40));
	
		return AD9850_RESULT_OK;
}


/*----------------------*/
/*     Internal API     */
/*----------------------*/

AD9850_Result ad9850_config(AD9850 *dds)
{
  AD9850_RETURN_ASSERT(dds != NULL, AD9850_RESULT_ERROR);
  AD9850_RETURN_ASSERT(dds->cfg.gpios != NULL, AD9850_RESULT_ERROR);
  AD9850_RETURN_ASSERT(dds->cfg.gpios->write != NULL, AD9850_RESULT_ERROR);

  AD9850_GPIO_Interface *const gpios = dds->cfg.gpios;

  for (unsigned i = 0; i < dds->dp_amount; ++i)
  {
    if (gpios->configure != NULL)
      AD9850_RETURN_IF_ERROR(gpios->configure(gpios, dds->dp_first + i, AD9850_PINMODE_OUTPUT));

    AD9850_RETURN_IF_ERROR(gpios->write(gpios, dds->dp_first + i, AD9850_PINSTATE_LOW));
  }

  AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_RESET, AD9850_PINSTATE_LOW));
  AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_W_CLK, AD9850_PINSTATE_LOW));
  AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_FQ_UD, AD9850_PINSTATE_LOW));

  return AD9850_RESULT_OK;
}

AD9850_Result ad9850_send(AD9850 *dds, uint64_t value, uint8_t bit_count)
{
  AD9850_RETURN_ASSERT(dds != NULL, AD9850_RESULT_ERROR);
  AD9850_RETURN_ASSERT(dds->cfg.gpios != NULL, AD9850_RESULT_ERROR);
  AD9850_RETURN_ASSERT(dds->cfg.gpios->write != NULL, AD9850_RESULT_ERROR);
  AD9850_RETURN_ASSERT((bit_count > 0) && (bit_count <=40), AD9850_RESULT_ERROR);

  AD9850_GPIO_Interface *const gpios = dds->cfg.gpios;

  for (unsigned i = 0; i < dds->dp_amount; ++i)
  {
    AD9850_RETURN_IF_ERROR(gpios->configure(gpios, dds->dp_first + i, AD9850_PINMODE_OUTPUT));
  }
	
	if (dds->cfg.mode == AD9850_MODE_PARALLEL)
  {
		uint8_t byte;
		
		for (unsigned i=0; i < ((bit_count + 7) >> 3); ++i)
    {
		  byte = (value >> ((4 - i) * 8)) & 0x00ff;
			for (unsigned j = 0; j < dds->dp_amount; ++j)
      {
        AD9850_RETURN_IF_ERROR(gpios->write(gpios, dds->dp_first + j, (byte >> j) & 0x01));
      }
  	  AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_W_CLK, AD9850_PINSTATE_HIGH));
      AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_W_CLK, AD9850_PINSTATE_LOW));
		}
	}	

  else  // SERIAL
  {
	  // Switch to serial mode D0 and D1 must be set by hardware
		AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_W_CLK, AD9850_PINSTATE_HIGH));
    AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_W_CLK, AD9850_PINSTATE_LOW));

		AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_FQ_UD, AD9850_PINSTATE_HIGH));
    AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_FQ_UD, AD9850_PINSTATE_LOW));

		for (unsigned i=0; i<bit_count; i++)
    {
		  AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_D7, (value >> i) & 0x01));
  	  AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_W_CLK, AD9850_PINSTATE_HIGH));
      AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_W_CLK, AD9850_PINSTATE_LOW));
		}
  }
	
	AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_FQ_UD, AD9850_PINSTATE_HIGH));
  AD9850_RETURN_IF_ERROR(gpios->write(gpios, AD9850_PIN_FQ_UD, AD9850_PINSTATE_LOW));

  return AD9850_RESULT_OK;
}
