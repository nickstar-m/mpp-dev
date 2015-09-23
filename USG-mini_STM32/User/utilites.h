/******************************************************************************
 * @file     utilites.h
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

#include "global.h"

void Delay_ms (uint16_t delay_ms);
void GetUptime(RTC_TimeTypeDef uptime);
void GetUptimeStr(char *uptime_str);
int32_t GetTemperature(void);
void USART_PutStr(char* str);
char* USART_GetStr(void);