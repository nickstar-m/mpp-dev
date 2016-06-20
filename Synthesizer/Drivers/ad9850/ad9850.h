#ifndef AD9850_H_
#define AD9850_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum { AD9850_RESULT_OK, AD9850_RESULT_ERROR } AD9850_Result;
typedef enum { AD9850_PINMODE_INPUT, AD9850_PINMODE_OUTPUT } AD9850_PinMode;
typedef enum { AD9850_PINSTATE_LOW, AD9850_PINSTATE_HIGH } AD9850_PinState;

/* Abstract AD9850 pin definition */
typedef enum
{
  AD9850_PIN_RESET,     // Reset. This is the master reset function; when set high, it clears all registers (except
                        // the input register), and the DAC output goes to cosine 0 after additional clock cycles
  AD9850_PIN_FQ_UD,     // Frequency Update. On the rising edge of this clock, the DDS updates to the frequency (or phase)
                        // loaded in the data input register; it then resets the pointer to Word 0.
  AD9850_PIN_W_CLK,     // Word Load Clock. This clock is used to load the parallel or serial frequency/phase/control words.

  /* Data pins D0..D7; in serial mode mode D0..D6 are not needed. */
  AD9850_PIN_D0,
  AD9850_PIN_D1,
  AD9850_PIN_D2,
  AD9850_PIN_D3,
  AD9850_PIN_D4,
  AD9850_PIN_D5,
  AD9850_PIN_D6,
  AD9850_PIN_D7,

  AD9850_PINS_AMOUNT // enum member counter, must be last
} AD9850_Pin;

/* Hardware-independent pin control interface.
 * configure() function is optional if you want to configure
 * the display pins manually.
 */
struct AD9850_GPIO_Interface_Struct;
typedef struct AD9850_GPIO_Interface_Struct AD9850_GPIO_Interface;

struct AD9850_GPIO_Interface_Struct
{
  AD9850_Result (*configure)(AD9850_GPIO_Interface *interface,
    AD9850_Pin pin, AD9850_PinMode mode);
  AD9850_Result (*write)(AD9850_GPIO_Interface *interface,
    AD9850_Pin pin, AD9850_PinState value);
  AD9850_Result (*read)(AD9850_GPIO_Interface *interface,
    AD9850_Pin pin, AD9850_PinState *value);
};

typedef void (*AD9850_AssertFn)(const char *filename, unsigned long line);

typedef enum { AD9850_MODE_SERIAL, AD9850_MODE_PARALLEL } AD9850_Mode;

/* Hardware abstraction layer */
typedef struct
{
  AD9850_Mode mode;
	AD9850_GPIO_Interface *gpios;
  AD9850_AssertFn assert_failure_handler;
} AD9850_Config;

/* AD9850 control structure */
typedef struct
{
  AD9850_Config cfg;
	uint64_t word40bit;
  AD9850_Pin dp_first;
  unsigned dp_amount;
} AD9850;

AD9850_Result ad9850_config(AD9850 *dds);
AD9850_Result ad9850_init(AD9850 *dds, const AD9850_Config *config);
AD9850_Result ad9850_reset(AD9850 *dds);
AD9850_Result ad9850_power_on(AD9850 *dds);
AD9850_Result ad9850_power_off(AD9850 *dds);
AD9850_Result ad9850_phase_update(AD9850 *dds, uint8_t phase);
AD9850_Result ad9850_frequency_update(AD9850 *dds, uint32_t frequency);
AD9850_Result ad9850_full_update(AD9850 *dds, uint8_t control, uint8_t phase, uint32_t frequency);
AD9850_Result ad9850_write_word(AD9850 *dds, uint64_t word);

#ifdef __cplusplus
}
#endif

#endif /* AD9850_H_ */
