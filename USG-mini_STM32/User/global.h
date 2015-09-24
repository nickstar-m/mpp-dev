/**************************************************************************//**
 * @file     global.h
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
	 
#ifndef GLOBAL_H
#define GLOBAL_H

#include "stm32f0xx.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// -----------------------------------
//  USART BUFFERS
// -----------------------------------
#if (INTERFACE_TYPE == TERMINAL)

  // USART_Buffer_TypeDef
	// USART Buffer processing structure
	typedef struct
	{
		char Buffer[128];                   /*!< Transmit USART Buffer FIFO */

		uint8_t Pointer;                    /*!< Pointer to current output position */

		uint8_t Size;                       /*!< Size of unprocessed data */
	} USART_BufferTypeDef;
	
	extern USART_BufferTypeDef RX_Buffer;
	extern USART_BufferTypeDef TX_Buffer;

#endif

// -----------------------------------
//  CONSTANTS OF DEVICE
// -----------------------------------
typedef struct
{
	char SERIAL[16];													/*!<  */
	char MODEL[16];  												  /*!<  */
	char PROD_DATE[8];	 											/*!<  */
	char REV_FW[16];													/*!<  */
	char REV_HW[16];													/*!<  */
	char PSERIAL[16];												  /*!<  */
	char PMODEL[16];													/*!<  */
	
} DevConstantsTypeDef;
	
// -----------------------------------
//  STATE OF DEVICE
// -----------------------------------
typedef struct
{
  uint16_t T_CASE;										/*!<  */
  uint16_t T_KEYS;										/*!<  */
  uint16_t VIN;												/*!<  */
  uint16_t V33;												/*!<  */
  uint16_t V15;												/*!<  */
  uint16_t HV;												/*!<  */
  uint16_t CG;												/*!<  */
  uint16_t CH;												/*!<  */
  RTC_TimeTypeDef UPTIME;							/*!<  */
  uint16_t HEAT_STATE;								/*!<  */
  uint16_t TRIAC_ANGLE;								/*!<  */
  uint16_t GEN_STATE;									/*!<  */
  uint16_t LAST_ERROR;								/*!<  */
	
} DevStateTypeDef;

// -----------------------------------
//  STATISTICS OF DEVICE
// -----------------------------------
typedef struct
{
  uint32_t TIME_ON;										/*!<  */
  uint32_t TIME_H;										/*!<  */
  uint32_t TIME_T;										/*!<  */
  uint32_t S_KWH; 										/*!<  */

} DevStatisticsTypeDef;

// -----------------------------------
//  CONTROL VARIABLES OF DEVICE
// -----------------------------------
typedef struct
{
  uint16_t TR_N;											/*!<  */
  uint16_t TR_PMAX;										/*!<  */
  uint16_t HT_PMAX;										/*!<  */
  uint32_t FR;												/*!<  */
  uint32_t FR_BW;											/*!<  */
  uint16_t US_P;											/*!<  */
  uint16_t HT_P;											/*!<  */
  uint16_t HEAT;											/*!<  */
  uint16_t GEN;												/*!<  */
  uint16_t SWP_DP;										/*!<  */
  uint16_t SWP_FR;										/*!<  */
  uint16_t SWP;												/*!<  */
  uint16_t PUL_FR;										/*!<  */
  uint16_t PUL_DC;										/*!<  */
  uint16_t PUL;												/*!<  */

} DevControlVariablesTypeDef;

// -----------------------------------
//  ADDITIONAL CONTROL VARIABLES OF DEVICE
// -----------------------------------
typedef struct
{
  uint16_t FR_S;											/*!<  */
  uint16_t SAVE_P;										/*!<  */

} DevAdditionalControlVariablesTypeDef;

// -----------------------------------
//  INTERNAL VARIABLES OF DEVICE
// -----------------------------------
typedef struct
{
  uint32_t* RES_FR[10];								/*!<  */
  uint32_t  CUR_FR;										/*!<  */

}  DevInternalVariablesTypeDef;

extern const DevConstantsTypeDef DevConstants;
extern DevStateTypeDef DevState;
extern DevStatisticsTypeDef DevStatistics;
extern DevControlVariablesTypeDef DevControlVariables;
extern DevAdditionalControlVariablesTypeDef DevAdditionalControlVariables;
	
extern volatile uint16_t delay_count;


	
#endif
