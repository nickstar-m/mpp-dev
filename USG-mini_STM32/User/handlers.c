/******************************************************************************
 * @file     handlers.c
 * @brief    USG-mini generator main board firmware
 * @version  V1.00
 * @date     23. September 2015
 * @author   Nickstar
 *
 * @note     Interrupt handlers
 *
 ******************************************************************************/
/* Copyright (c) 2015 MedPromPrylad

   ---------------------------------------------------------------------------*/
# include "handlers.h"


void SysTick_Handler (void)
{
  if (delay_count) delay_count--;
}

void DMA1_Channel2_3_IRQHandler (void)
{

}

void DMA1_Channel5_6_IRQHandler (void)
{

}
