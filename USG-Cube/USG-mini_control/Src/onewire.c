/*
 * onewire.c
 *
 *  Created on: 13.02.2012
 *      Author: di
 */

#include "onewire.h"

#ifdef OW_USART1

#undef OW_USART2
#undef OW_USART3
#undef OW_USART4

#define OW_USART 		USART1
#define OW_DMA_CH_RX 	DMA1_Channel5
#define OW_DMA_CH_TX 	DMA1_Channel4
#define OW_DMA_FLAG		DMA1_FLAG_TC5

#endif


#ifdef OW_USART2

#undef OW_USART1
#undef OW_USART3
#undef OW_USART4

#define OW_USART 		USART2
#define OW_DMA_CH_RX 	DMA1_Channel6
#define OW_DMA_CH_TX 	DMA1_Channel7
#define OW_DMA_FLAG		DMA1_FLAG_TC6

#endif


uint8_t ow_buf[8];
static uint8_t uart_busy;

#define OW_0	0x00
#define OW_1	0xff
#define OW_R_1	0xff

void OW_toBits(uint8_t ow_byte, uint8_t *ow_bits) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		if (ow_byte & 0x01) {
			*ow_bits = OW_1;
		} else {
			*ow_bits = OW_0;
		}
		ow_bits++;
		ow_byte = ow_byte >> 1;
	}
}

uint8_t OW_toByte(uint8_t *ow_bits) {
	uint8_t ow_byte, i;
	ow_byte = 0;
	for (i = 0; i < 8; i++) {
		ow_byte = ow_byte >> 1;
		if (*ow_bits == OW_R_1) {
			ow_byte |= 0x80;
		}
		ow_bits++;
	}

	return ow_byte;
}

uint8_t OW_Init(UART_HandleTypeDef *huart) {
	huart->Init.BaudRate = 115200;
	UART_SetConfig(huart);
	return OW_OK;
}

uint8_t OW_Reset(UART_HandleTypeDef *huart) {
	uint8_t tx_buffer[1];
	uint8_t rx_buffer[1];

	huart->Init.BaudRate = 9600;
	UART_SetConfig(huart);

	tx_buffer[0] = 0xf0;
	HAL_UART_Transmit_DMA(huart, tx_buffer, sizeof(tx_buffer));
	HAL_UART_Receive_DMA(huart, rx_buffer, sizeof(rx_buffer));
  uart_busy = 1;
	while (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);
	UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_TC, SET, 100);
	
	huart->Init.BaudRate = 115200;
	UART_SetConfig(huart);

	if (rx_buffer[0] != 0xf0) {
		return OW_OK;
	}

	return OW_NO_DEVICE;
}

uint8_t OW_Send(UART_HandleTypeDef *huart, uint8_t sendReset, uint8_t *command, uint8_t cLen,
		uint8_t *data, uint8_t dLen, uint8_t readStart) {

	if (sendReset == OW_SEND_RESET) {
		if (OW_Reset(huart) == OW_NO_DEVICE) {
			return OW_NO_DEVICE;
		}
	}

	while (cLen > 0) {

		OW_toBits(*command, ow_buf);
		command++;
		cLen--;

		HAL_UART_Transmit_DMA(huart, ow_buf, 8);
		HAL_UART_Receive_DMA(huart, ow_buf, 8);
		UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_PE, SET, 100);

		if (readStart == 0 && dLen > 0) {
			*data = OW_toByte(ow_buf);
			data++;
			dLen--;
		} else {
			if (readStart != OW_NO_READ) {
				readStart--;
			}
		}
	}

	return OW_OK;
}

