/**************************************************************************//**
 * @file     periph_init.h
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

#ifndef PERIPH_INIT_H
  #define PERIPH_INIT_H

	#include "stm32f0xx.h"
	#include "global.h"
	
	void GPIO_init (void);
	void USART_init_ (void);
  void DMA_USART_TX_init (void);
	void ADC_init (void);
	void RTC_init (void);
  
#endif
