/******************************************************************************
 * @file     periph_init.c
 * @brief    USG-mini generator main board firmware
 * @version  V1.00
 * @date     21. September 2015
 * @author   Nickstar
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2015 MedPromPrylad

   ---------------------------------------------------------------------------*/
	 
#include "periph_init.h"                // Init header

// ---------------------------------------------------
// GPIO init proc
// ---------------------------------------------------
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

// ---------------------------------------------------
// USART init proc
// ---------------------------------------------------
#if (INTERFACE_TYPE == TERMINAL)
	void USART_init_ (void)
	{
		GPIO_InitTypeDef GPIO_init_USART;
		USART_InitTypeDef USART_init_d;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
		GPIO_init_USART.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
		GPIO_init_USART.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_init_USART.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_init_USART.GPIO_OType = GPIO_OType_PP;
		GPIO_init_USART.GPIO_PuPd  = GPIO_PuPd_UP;
		
		GPIO_Init(GPIOA, &GPIO_init_USART);
		
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
		
		USART_init_d.USART_BaudRate            = 115200;
		USART_init_d.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_init_d.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
		USART_init_d.USART_Parity              = USART_Parity_No;
		USART_init_d.USART_StopBits            = USART_StopBits_1;
		USART_init_d.USART_WordLength          = USART_WordLength_8b;
		
		USART_Init(USART1, &USART_init_d);
		
		USART_Cmd(USART1, ENABLE);

	}

#endif

// ---------------------------------------------------
// I2C init proc
// ---------------------------------------------------
#if (INTERFACE_TYPE == I2C)

#endif

// ---------------------------------------------------
// DMA init proc
// ---------------------------------------------------
#if (INTERFACE_TYPE == TERMINAL)
	void DMA_USART_TX_init(void){
		DMA_InitTypeDef DMA_init_USART;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		DMA_init_USART.DMA_PeripheralBaseAddr = (uint32_t)&USART1->TDR;
		DMA_init_USART.DMA_MemoryBaseAddr     = (uint32_t)TX_buffer;
		DMA_init_USART.DMA_DIR                = DMA_DIR_PeripheralDST;
		DMA_init_USART.DMA_BufferSize         = sizeof(TX_buffer);
		DMA_init_USART.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
		DMA_init_USART.DMA_MemoryInc          = DMA_MemoryInc_Enable;
		DMA_init_USART.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_init_USART.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
		DMA_init_USART.DMA_Mode               = DMA_Mode_Normal;
		DMA_init_USART.DMA_Priority           = DMA_Priority_Medium;
		DMA_init_USART.DMA_M2M                = DMA_M2M_Disable;
		
		DMA_Init(DMA1_Channel2, &DMA_init_USART);
	}

	void DMA_USART_RX_init(void){
		DMA_InitTypeDef DMA_init_USART;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		DMA_init_USART.DMA_PeripheralBaseAddr = (uint32_t)&USART1->RDR;
		DMA_init_USART.DMA_MemoryBaseAddr     = (uint32_t)RX_buffer;
		DMA_init_USART.DMA_DIR                = DMA_DIR_PeripheralSRC;
		DMA_init_USART.DMA_BufferSize         = sizeof(RX_buffer);
		DMA_init_USART.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
		DMA_init_USART.DMA_MemoryInc          = DMA_MemoryInc_Enable;
		DMA_init_USART.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_init_USART.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
		DMA_init_USART.DMA_Mode               = DMA_Mode_Normal;
		DMA_init_USART.DMA_Priority           = DMA_Priority_Medium;
		DMA_init_USART.DMA_M2M                = DMA_M2M_Disable;
		
		DMA_Init(DMA1_Channel2, &DMA_init_USART);
	}

#endif
	
// ---------------------------------------------------
// RTC init proc
// ---------------------------------------------------

// ---------------------------------------------------
// TIMER init proc
// ---------------------------------------------------

// ---------------------------------------------------
// IRQ init proc
// ---------------------------------------------------
