/*
 * FreeModbus Libary: AVR Demo Application
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: demo.c,v 1.7 2006/06/15 15:38:02 wolti Exp $
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "stm32f0xx.h"   
#include "mb.h"
#include "mbport.h"
#include "init_periph.h"

// include Playing With Fusion MAX31865 libraries
#include <PlayingWithFusion_MAX31865.h>              // core library
//#include <PlayingWithFusion_MAX31865_STRUCT.h>       // struct library
/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4

/* ----------------------- Static variables ---------------------------------*/
//static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];

static struct var_max31865 RTD_CH0;
static struct var_max31865 RTD_CH1;

/* ----------------------- Start implementation -----------------------------*/
int
main( void )
{	UCHAR adress=0;
    const UCHAR     ucSlaveID[] = {"MPP_Logic_V10"};
    eMBErrorCode    eStatus;
    __disable_irq ();
    GPIO_init();
    ADC_init();
    SPI_init();
    PWFusion_MAX31865_RTD(0);
    MAX31865_config();
    PWFusion_MAX31865_RTD(1);
    MAX31865_config();

    RTD_CH0.RTD_type = 1;                         // un-comment for PT100 RTD
    // RTD_CH0.RTD_type = 2;                        // un-comment for PT1000 RTD
    RTD_CH1.RTD_type = 1;                         // un-comment for PT100 RTD
    // RTD_CH0.RTD_type = 2;                        // un-comment for PT1000 RTD
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)) adress=adress+1;
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)) adress=adress+2;
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13)) adress=adress+4;
    adress = adress + 32;

    eStatus = eMBInit( MB_RTU, adress, 0, 57600, MB_PAR_EVEN );

    eStatus = eMBSetSlaveID( 0x0A, TRUE, ucSlaveID, (sizeof(ucSlaveID)-1));

    __enable_irq ();
    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );

    for( ;; )
    {
        ( void )eMBPoll(  );
        if (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET) GPIO_SetBits(GPIOF,GPIO_Pin_5); else GPIO_ResetBits(GPIOF,GPIO_Pin_5);
        /* Here we simply count the number of poll cycles. */
        usRegInputBuf[0]++;
    }
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{

    eMBErrorCode    eStatus = MB_ENOERR;
    short             iRegIndex;

    if((usAddress>=2010) && (usAddress<=2013) && ((usAddress+usNRegs-1)<=2013))
    {
        while( usNRegs > 0 )
        {	switch(usAddress)
		{
		case 2010:
		ADC1->CHSELR = (uint32_t)ADC_Channel_0;
	    ADC_ChannelConfig(ADC1, ADC_Channel_0,ADC_SampleTime_55_5Cycles);
		// начинаем работу
	    ADC_StartOfConversion(ADC1);
		// ждём пока преобразуется напряжение в код
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		// возвращаем результат
		iRegIndex=ADC_GetConversionValue(ADC1);
		break;

		case 2011:
		ADC1->CHSELR = (uint32_t)ADC_Channel_1;
		ADC_ChannelConfig(ADC1, ADC_Channel_1,ADC_SampleTime_55_5Cycles);
		// начинаем работу
		ADC_StartOfConversion(ADC1);
		// ждём пока преобразуется напряжение в код
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		// возвращаем результат
		iRegIndex=ADC_GetConversionValue(ADC1);
		break;

		case 2012:
		PWFusion_MAX31865_RTD(0);
		MAX31865_full_read(&RTD_CH0);
		iRegIndex=(short)(((double)RTD_CH0.rtd_res_raw / 32) - 256);
		break;

		case 2013:
		PWFusion_MAX31865_RTD(1);
		MAX31865_full_read(&RTD_CH1);
		iRegIndex=(short)(((double)RTD_CH1.rtd_res_raw / 32) - 256);
		break;
		}
	    *pucRegBuffer++ =
                ( unsigned char )( iRegIndex >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( iRegIndex & 0xFF );
         usNRegs--;
	 usAddress++;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{	short temp;

	if ((usAddress >= 2000) && (usAddress <= 2007) && ((usAddress+usNRegs-1)<=2007) && (eMode == MB_REG_WRITE))
	{
        while( usNRegs > 0 )
        {
	temp=0;
	temp=*pucRegBuffer * 256;
	pucRegBuffer++;
	temp=temp + *pucRegBuffer;
	pucRegBuffer++;

	switch (usAddress){

	case 2000:
	if (temp) GPIO_SetBits(GPIOA,GPIO_Pin_14); else GPIO_ResetBits(GPIOA,GPIO_Pin_14);
	break;

	case 2001:
	if (temp) GPIO_SetBits(GPIOA,GPIO_Pin_15); else GPIO_ResetBits(GPIOA,GPIO_Pin_15);
	break;

	case 2002:
	if (temp) GPIO_SetBits(GPIOC,GPIO_Pin_10); else GPIO_ResetBits(GPIOC,GPIO_Pin_10);
	break;

	case 2003:
	if (temp) GPIO_SetBits(GPIOC,GPIO_Pin_11); else GPIO_ResetBits(GPIOC,GPIO_Pin_11);
	break;

	case 2004:
	if (temp) GPIO_SetBits(GPIOC,GPIO_Pin_12); else GPIO_ResetBits(GPIOC,GPIO_Pin_12);
	break;

	case 2005:
	if (temp) GPIO_SetBits(GPIOD,GPIO_Pin_2); else GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	break;

	case 2006:
	if (temp) GPIO_SetBits(GPIOB,GPIO_Pin_3); else GPIO_ResetBits(GPIOB,GPIO_Pin_3);
	break;

	case 2007:
	if (temp) GPIO_SetBits(GPIOB,GPIO_Pin_4); else GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	break;
/*
	case 2008:
	if (!temp) GPIO_SetBits(GPIOF,GPIO_Pin_5); else GPIO_ResetBits(GPIOF,GPIO_Pin_5);
	break;

	case 2009:
	if (!temp) GPIO_SetBits(GPIOA,GPIO_Pin_4); else GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	break;*/
	}

        usNRegs--;
	    usAddress++;
        }
    return MB_ENOERR;
	}

	else
	{
    return MB_ENOREG;
	}
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}
