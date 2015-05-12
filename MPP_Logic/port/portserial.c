/*
 * FreeModbus Libary: ATMega168 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *   - Initial version and ATmega168 support
 * Modfications Copyright (C) 2006 Tran Minh Hoang:
 *   - ATmega8, ATmega16, ATmega32 support
 *   - RS485 support for DS75176
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
 * File: $Id: portserial.c,v 1.6 2006/09/17 16:45:53 wolti Exp $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"


void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{	if(TRUE==xRxEnable)
{
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
else
{
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
}

if(TRUE==xTxEnable)
{
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	RTS_LOW;
}
else
{
   USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	RTS_HIGH;
}
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitStructure.USART_BaudRate = (uint32_t)ulBaudRate;

	switch (eParity)
	{
		case MB_PAR_EVEN:
			USART_InitStructure.USART_Parity = USART_Parity_Even;
			ucDataBits++;
			break;

		case MB_PAR_ODD:
			USART_InitStructure.USART_Parity = USART_Parity_Odd;
			ucDataBits++;
			break;

		case MB_PAR_NONE:
			USART_InitStructure.USART_Parity = USART_Parity_No;
			break;
	}

	switch (ucDataBits)
	{
		case 7:
			USART_InitStructure.USART_WordLength = USART_WordLength_7b;
			break;

		case 8:
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			break;

		case 9:
			USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			break;
	}
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);

	NVIC_EnableIRQ(USART1_IRQn);
	/* ENABLE the USARTx */
	USART_Cmd(USART1, ENABLE);

	#ifdef RTS_ENABLE
			RTS_INIT;
	#endif
			return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{ 
  unsigned char * temp;
	temp=(unsigned char *)(&ucByte);
	USART_SendData(USART1, (uint16_t)(*temp));
  return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{	
	unsigned char * temp;
	temp=(unsigned char *)pucByte;
	*temp = (unsigned char)USART_ReceiveData(USART1);
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  return TRUE;
}


void USART1_IRQHandler (void)
{	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{ 
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		pxMBFrameCBByteReceived(  );
	}

	if (USART_GetITStatus(USART1, USART_IT_TXE) == SET)
	{
  	USART_ClearITPendingBit(USART1, USART_IT_TXE);
   	pxMBFrameCBTransmitterEmpty(  );
	}
}
