/*
 * onewire.h
 *
 *  Version 1.0.1
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include "stm32f0xx.h"

#define OW_USART1
//#define OW_USART2
//#define OW_USART3
//#define OW_USART4


//#define OW_GIVE_TICK_RTOS

#define OW_SEND_RESET		1
#define OW_NO_RESET		2

#define OW_OK			1
#define OW_ERROR		2
#define OW_NO_DEVICE	3

#define OW_NO_READ		0xff

#define OW_READ_SLOT	0xff

uint8_t OW_Init(UART_HandleTypeDef *huart);
uint8_t OW_Send(UART_HandleTypeDef *huart, uint8_t sendReset, uint8_t *command, uint8_t cLen, uint8_t *data, uint8_t dLen, uint8_t readStart);
uint8_t OW_Reset(UART_HandleTypeDef *huart);
#endif /* ONEWIRE_H_ */
