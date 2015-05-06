#include "stm32f0xx.h"                  // Device header

uint8_t display_buffer[6] = {255};

// GPIO init proc
void GPIO_init (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	// Enable GPIO Peripheral clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	// Configure pin in output push/pull mode
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

 	// Configure pin in output push/pull mode
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// Timer TIM6 init proc
void TIM6_init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

}

// Display & keyboard handler
// based on timer TIM4
// Display driver
void display_driver()
{
	
}	


// Keyboard driver
void kbd_driver()
{

}

// Beep function
// frequency in Hz, duration in msec
void beep(uint32_t frequency, uint32_t duration)
{
	
}	

int main(void)
{
	uint32_t i;
	GPIO_init();
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
  GPIO_ResetBits(GPIOA,GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_ResetBits(GPIOB,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	
	while (1)
	{
	  for (i=1;i<2000000;i++){};
		GPIO_ResetBits(GPIOA, GPIO_Pin_14 | GPIO_Pin_15);
	  for (i=1;i<2000000;i++){};
		GPIO_SetBits(GPIOA, GPIO_Pin_14 | GPIO_Pin_15);
	}
}
