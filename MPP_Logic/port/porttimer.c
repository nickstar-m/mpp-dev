/*
 * FreeModbus Libary: ATMega168 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.4 2006/09/03 11:53:10 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "stm32f0xx.h"
/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint16_t PrescalerValue = 0;

	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 20000) - 1; // 1/20000=50us
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) usTim1Timerout50us;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	NVIC_EnableIRQ(TIM3_IRQn);
	/* TIM IT DISABLE */
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	/* TIM3 DISABLE counter */
	TIM_Cmd(TIM3,  DISABLE);
	return TRUE;;
}


inline void
vMBPortTimersEnable(  )
{
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_SetCounter(TIM3,0x0000);
  TIM_Cmd(TIM3, ENABLE);
}

inline void
vMBPortTimersDisable(  )
{
	TIM_Cmd(TIM3, DISABLE);
	TIM_SetCounter(TIM3,0x0000);
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}


void
TIM3_IRQHandler( )
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
	  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	  pxMBPortCBTimerExpired(  );
	  GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
}
