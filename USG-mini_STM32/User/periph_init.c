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
		DMA_InitTypeDef DMA_init_USART_struct;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		DMA_init_USART_struct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->TDR;
		DMA_init_USART_struct.DMA_MemoryBaseAddr     = (uint32_t)TX_Buffer.Buffer;
		DMA_init_USART_struct.DMA_DIR                = DMA_DIR_PeripheralDST;
		DMA_init_USART_struct.DMA_BufferSize         = sizeof(TX_Buffer.Buffer);
		DMA_init_USART_struct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
		DMA_init_USART_struct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
		DMA_init_USART_struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_init_USART_struct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
		DMA_init_USART_struct.DMA_Mode               = DMA_Mode_Normal;
		DMA_init_USART_struct.DMA_Priority           = DMA_Priority_Medium;
		DMA_init_USART_struct.DMA_M2M                = DMA_M2M_Disable;
		
		DMA_Init(DMA1_Channel2, &DMA_init_USART_struct);
	}

	void DMA_USART_RX_init(void){
		DMA_InitTypeDef DMA_init_USART_struct;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		DMA_init_USART_struct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->RDR;
		DMA_init_USART_struct.DMA_MemoryBaseAddr     = (uint32_t)TX_Buffer.Buffer;
		DMA_init_USART_struct.DMA_DIR                = DMA_DIR_PeripheralSRC;
		DMA_init_USART_struct.DMA_BufferSize         = sizeof(RX_Buffer.Buffer);
		DMA_init_USART_struct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
		DMA_init_USART_struct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
		DMA_init_USART_struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_init_USART_struct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
		DMA_init_USART_struct.DMA_Mode               = DMA_Mode_Normal;
		DMA_init_USART_struct.DMA_Priority           = DMA_Priority_Medium;
		DMA_init_USART_struct.DMA_M2M                = DMA_M2M_Disable;
		
		DMA_Init(DMA1_Channel2, &DMA_init_USART_struct);
	}

#endif

// ---------------------------------------------------
// USART init proc
// ---------------------------------------------------
void ADC_init (void)
{
  GPIO_InitTypeDef GPIO_Init_struct;
  ADC_InitTypeDef  ADC_Init_struct;

  //(#) Enable the ADC interface clock using
  //    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  //(#) ADC pins configuration
  //   (++) Enable the clock for the ADC GPIOs using the following function:
  //        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOx, ENABLE);  
  //   (++) Configure these ADC pins in analog mode using GPIO_Init();  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_Init_struct.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init_struct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_Init_struct);

  //(#) Configure the ADC conversion resolution, data alignment, external
  //    trigger and edge, scan direction and Enable/Disable the continuous mode
  //    using the ADC_Init() function.
  ADC_Init_struct.ADC_Resolution = ADC_Resolution_12b;
  ADC_Init_struct.ADC_ContinuousConvMode = DISABLE;
  ADC_Init_struct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;    
  ADC_Init_struct.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_Init_struct.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_Init_struct);

  // Calibrate ADC before enabling
  ADC_GetCalibrationFactor(ADC1);
  //(#) Activate the ADC peripheral using ADC_Cmd() function.
  ADC_Cmd(ADC1, ENABLE);

  // Wait until ADC enabled
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET);
}	
	
// ---------------------------------------------------
// RTC init proc
// ---------------------------------------------------
void RTC_init (void)
{
  RTC_InitTypeDef RTC_init_struct;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
 
  PWR_BackupAccessCmd(ENABLE);

  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
 
  RCC_LSICmd(ENABLE);
  RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div32);
  RCC_RTCCLKCmd(ENABLE);
 
  RTC_init_struct.RTC_HourFormat = RTC_HourFormat_24;
  RTC_init_struct.RTC_AsynchPrediv = 124;
  RTC_init_struct.RTC_SynchPrediv = 1999;
  RTC_Init(&RTC_init_struct);
}

// ---------------------------------------------------
// TIMER init proc
// ---------------------------------------------------

// ---------------------------------------------------
// NVIC init proc
// ---------------------------------------------------
void NVIC_init(void){
  NVIC_InitTypeDef NVIC_InitStructure;
	// Enable the USART RX Interrupt 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
