/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* USER CODE BEGIN Includes */
//#include "hd44780.h"
#include "hd44780_stm32f0xx.h"
//#include "ad9850.h"
#include "ad9850_stm32f0xx.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
HD44780 lcd;
HD44780_STM32F0xx_GPIO_Driver lcd_pindriver;
volatile uint32_t systick_ms = 0;
AD9850 dds;
AD9850_STM32F0xx_GPIO_Driver dds_pindriver;
uint16_t encoder_event;
uint64_t frequency = 2000000;

/* Encoder accuracy */
typedef enum
{
	EXTRAFINE   = 0x00,
	FINE        = 0x01,
	NORMAL      = 0x02,
	COARSE      = 0x03,
	EXTRACOARSE = 0x04,
} EncoderAccuracyTypeDef;

EncoderAccuracyTypeDef EncoderAccuracy = EXTRAFINE;

const unsigned EncoderStep[] = {1, 5, 9, 13, 17};  /* {EXTRAFINE, FINE, NORMAL, COARSE, EXTRACOARSE}    frequency +- (1 << x) */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void init_lcd(void);
void delay_microseconds(uint16_t us);
uint32_t uint32_time_diff(uint32_t now, uint32_t before);
void hd44780_assert_failure_handler(const char *filename, unsigned long line);
void init_AD9850(void);
void ad9850_assert_failure_handler(const char *filename, unsigned long line);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
#define CW  0
#define CCW 1
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();

  /* USER CODE BEGIN 2 */
	init_lcd();
	hd44780_clear(&lcd);
	
	init_AD9850();
	
	HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	uint8_t steps;
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    
		static uint32_t lcd_update_ms = 0;

    if (uint32_time_diff(HAL_GetTick(), lcd_update_ms) >= 10)
    {
      lcd_update_ms = HAL_GetTick();

      static unsigned counter = 0;

      
			if (encoder_event > 0)
			{
        const size_t buf_size = lcd.columns_amount + 1;
        char buf[buf_size];

        snprintf(buf, buf_size, "F = %llu.%02llu Hz", frequency / 100, frequency % 100);

        ++counter;

        hd44780_move_cursor(&lcd, 0, 0);
        hd44780_write_string(&lcd, buf);

			  if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3) == CW)
          frequency += 1 << EncoderStep[EncoderAccuracy];
				else
          frequency -= 1 << EncoderStep[EncoderAccuracy];
				
				if (frequency < 1000 || frequency > 8000000000) frequency = 1000;
				snprintf(buf, buf_size, "Enc_val: %u:%s ", encoder_event/*__HAL_TIM_GET_COUNTER(&htim3)*/, __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3) == CW ? "CW" : "CCW");
			  hd44780_move_cursor(&lcd, 0, 1);
        hd44780_write_string(&lcd, buf);
			
			  ad9850_frequency_update(&dds, frequency);
				encoder_event = 0;
			}	
    }
  
  }
	
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** NVIC Configuration
*/
static void MX_NVIC_Init(void)
{
  /* TIM3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_Encoder_InitTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 255;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 1;
  sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 1;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, W_CLK_Pin|FQ_UD_Pin|D0_Pin|D1_Pin 
                          |D2_Pin|D3_Pin|D4_Pin|D5_Pin 
                          |D6_Pin|D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RESET_Pin|RS_Pin|R_W_Pin|E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : W_CLK_Pin FQ_UD_Pin D0_Pin D1_Pin 
                           D2_Pin D3_Pin D4_Pin D5_Pin 
                           D6_Pin D7_Pin */
  GPIO_InitStruct.Pin = W_CLK_Pin|FQ_UD_Pin|D0_Pin|D1_Pin 
                          |D2_Pin|D3_Pin|D4_Pin|D5_Pin 
                          |D6_Pin|D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SWITCH_Pin */
  GPIO_InitStruct.Pin = SWITCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SWITCH_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RESET_Pin RS_Pin R_W_Pin E_Pin */
  GPIO_InitStruct.Pin = RESET_Pin|RS_Pin|R_W_Pin|E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_PA9);

  /**/
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_PA10);

}

/* USER CODE BEGIN 4 */

void init_lcd(void)
{
  const HD44780_STM32F0xx_Pinout lcd_pinout =
  {
    {
      /* RS        */  { GPIOB, GPIO_PIN_3 },
      /* ENABLE    */  { GPIOB, GPIO_PIN_5 },
      /* RW        */  { GPIOB, GPIO_PIN_4 },
      /* Backlight */  { NULL, 0 },
      /* DP0       */  { GPIOA, GPIO_PIN_8 },
      /* DP1       */  { GPIOA, GPIO_PIN_9 },
      /* DP2       */  { GPIOA, GPIO_PIN_10 },
      /* DP3       */  { GPIOA, GPIO_PIN_11 },
      /* DP4       */  { GPIOA, GPIO_PIN_12 },
      /* DP5       */  { GPIOA, GPIO_PIN_13 },
      /* DP6       */  { GPIOA, GPIO_PIN_14 },
      /* DP7       */  { GPIOA, GPIO_PIN_15 },
    }
  };

  lcd_pindriver.interface = HD44780_STM32F0XX_PINDRIVER_INTERFACE;
  lcd_pindriver.pinout = lcd_pinout;
  lcd_pindriver.assert_failure_handler = hd44780_assert_failure_handler;

  const HD44780_Config lcd_config =
  {
    (HD44780_GPIO_Interface*)&lcd_pindriver,
    delay_microseconds,
    hd44780_assert_failure_handler,
    HD44780_OPT_USE_RW
  };

  hd44780_init(&lcd, HD44780_MODE_8BIT, &lcd_config, 16, 2, HD44780_CHARSIZE_5x8);
}

void init_AD9850(void)
{
  const AD9850_STM32F0xx_Pinout dds_pinout =
  {
    {
      /* RESET     */  { GPIOB, GPIO_PIN_1 },
      /* FQ_UD     */  { GPIOA, GPIO_PIN_5 },
      /* W_CLK     */  { GPIOA, GPIO_PIN_4 },
      /* D0        */  { GPIOA, GPIO_PIN_8 },
      /* D1        */  { GPIOA, GPIO_PIN_9 },
      /* D2        */  { GPIOA, GPIO_PIN_10 },
      /* D3        */  { GPIOA, GPIO_PIN_11 },
      /* D4        */  { GPIOA, GPIO_PIN_12 },
      /* D5        */  { GPIOA, GPIO_PIN_13 },
      /* D6        */  { GPIOA, GPIO_PIN_14 },
      /* D7        */  { GPIOA, GPIO_PIN_15 },
    }
  };

  dds_pindriver.interface = AD9850_STM32F0XX_PINDRIVER_INTERFACE;
  dds_pindriver.pinout = dds_pinout;
  dds_pindriver.assert_failure_handler = ad9850_assert_failure_handler;

  const AD9850_Config dds_config =
  {
    125000000,              // clock frequency
		AD9850_MODE_PARALLEL,
		(AD9850_GPIO_Interface*)&dds_pindriver,
    ad9850_assert_failure_handler
  };

  ad9850_init(&dds, &dds_config);
	
}

void delay_microseconds(uint16_t us)
{
  HAL_Delay(((uint32_t)us / 1000) + (us % 1000 ? 1 : 0));
}

uint32_t uint32_time_diff(uint32_t now, uint32_t before)
{
  return (now >= before) ? (now - before) : (UINT32_MAX - before + now);
}

void hd44780_assert_failure_handler(const char *filename, unsigned long line)
{
  (void)filename; (void)line;
  do {} while (1);
}

void ad9850_assert_failure_handler(const char *filename, unsigned long line)
{
  (void)filename; (void)line;
  do {} while (1);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t PreviousEncoderValue;
	static uint32_t PreviousTick = 0;
	static uint16_t EncoderAverageStepTime = 64 * 2;
	uint16_t step_time;
	uint16_t average_time;

	
	if (__HAL_TIM_GET_COUNTER(htim) != PreviousEncoderValue)
	{
	  step_time = (HAL_GetTick() - PreviousTick) > 500 ? 500 : HAL_GetTick() - PreviousTick;
    
		if (step_time < 500) // < 0.5 sec
		{
      average_time = EncoderAverageStepTime >> 1;
			EncoderAverageStepTime = EncoderAverageStepTime - average_time + step_time;
			if (average_time > 128)
			  EncoderAccuracy = EXTRAFINE;
			else
        if (average_time > 64)
          EncoderAccuracy = FINE;
        else  
				  if (average_time > 32)
            EncoderAccuracy = NORMAL;
					else
				    if (average_time > 16)
              EncoderAccuracy = COARSE;
					  else
              EncoderAccuracy = EXTRACOARSE;
		}
    else
      if (EncoderAccuracy > FINE)
      {
				EncoderAccuracy = FINE;
				EncoderAverageStepTime = 128 * 2;
			}	
    
		if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3) == CW)
		{
		  if (__HAL_TIM_GET_COUNTER(htim) > PreviousEncoderValue)
				encoder_event = __HAL_TIM_GET_COUNTER(htim) - PreviousEncoderValue;
			else
				encoder_event = 256 + __HAL_TIM_GET_COUNTER(htim) - PreviousEncoderValue;
		}	
		else
		{
		  if (__HAL_TIM_GET_COUNTER(htim) < PreviousEncoderValue)
				encoder_event = PreviousEncoderValue - __HAL_TIM_GET_COUNTER(htim);
			else
				encoder_event = 256 + PreviousEncoderValue - __HAL_TIM_GET_COUNTER(htim);
		}
		PreviousEncoderValue = __HAL_TIM_GET_COUNTER(htim);
    PreviousTick = HAL_GetTick();
	}	
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
