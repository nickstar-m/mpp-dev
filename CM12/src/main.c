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
#include "mb.h"
#include "mbport.h"
#include "stm32f0xx.h"
#include "init_periph.h"
/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4

#define V_NULL 3010
#define V_DREYF 5
/* ----------------------- Static variables ---------------------------------*/
//static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];

short ADC_50_Hz(ADC_TypeDef* ADCx, uint32_t ADC_Channel)
		{
	uint16_t temp=0, last_temp=0, i=0, count=0;
	uint32_t  result=0;
	uint32_t mas[10][2], index=0;

	ADCx->CHSELR = (uint32_t)ADC_Channel;
    ADC_ChannelConfig(ADCx, ADC_Channel,ADC_SampleTime_55_5Cycles);

    for(i=0;i<4000;i++){
	// начинаем работу
    ADC_StartOfConversion(ADCx);
	// ждём пока преобразуется напряжение в код
	while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
	// возвращаем результат
	last_temp=temp;
	temp=ADC_GetConversionValue(ADCx);
	if ((i) && ((temp > ((uint32_t)last_temp + ((uint32_t)last_temp/10))) || (temp < (last_temp - (last_temp/10))))) temp = last_temp; //перевірка чи число не виходить за межі 10%
	if (temp <= V_NULL) {result+=(V_NULL-temp); count++;} //якщо це нижня півхвиля сумуємо значення виборок і рахуємо кількість виборок
	if ((temp > V_NULL + V_DREYF) && (count)) {if (count > 400) {mas[index][0]=result; mas[index][1]=count; index++;} count=0; result=0;} //Якщо це верхня півхвиля, запамятовуємо значення суми виборок і кількості виборок в масив, обнуляємо їх значення
    }
    if (index){//вибираю повну півхвилю
    mas[index][0]=result;
    mas[index][1]=count;
    while(index){
    index--;
    if (mas[index][1] > mas[index+1][1]) {result=mas[index][0]; count=mas[index][1];}
    }
    }
    if (count) return (short)((result/count)*8+200); else return -1;//повертаю значення у мА, або -1 у випадку відсутності струму, або протікання постійного струму позитивної полярності.
    														   //у випадку відсутності струму може повертатісь мале значення струму
	}
/* ----------------------- Start implementation -----------------------------*/
int
main( void )
{	UCHAR adress=0;
    const UCHAR     ucSlaveID[] = {"CM12"};
    eMBErrorCode    eStatus;
    __disable_irq ();
    GPIO_init();
    ADC_init();

    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)) adress=adress+1;
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)) adress=adress+2;
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13)) adress=adress+4;
    adress=adress+40;

    eStatus = eMBInit( MB_RTU, adress, 0, 57600, MB_PAR_EVEN );

    eStatus = eMBSetSlaveID( 0x0A, TRUE, ucSlaveID, (sizeof(ucSlaveID)-1));

    __enable_irq ();
    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );

    for( ;; )
    {
        ( void )eMBPoll(  );
        if (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET) GPIO_SetBits(GPIOC,GPIO_Pin_11); else GPIO_ResetBits(GPIOC,GPIO_Pin_11);
        //if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) GPIO_SetBits(GPIOC,GPIO_Pin_12); else GPIO_ResetBits(GPIOC,GPIO_Pin_12);
        /* Here we simply count the number of poll cycles. */
        usRegInputBuf[0]++;
    }
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{

    eMBErrorCode    eStatus = MB_ENOERR;
    short             iRegIndex;

    if((usAddress>=3000) && (usAddress<=3011) && ((usAddress+usNRegs-1)<=3011))
    {
        while( usNRegs > 0 )
        {	switch(usAddress)
		{
		case 3000:
		iRegIndex=ADC_50_Hz(ADC1, ADC_Channel_0);
		break;

		case 3001:
		iRegIndex=ADC_50_Hz(ADC1, ADC_Channel_1);
		break;

		case 3002:
		iRegIndex=ADC_50_Hz(ADC1, ADC_Channel_2);
		break;

		case 3003:
		iRegIndex=ADC_50_Hz(ADC1, ADC_Channel_3);
		break;

		case 3004:
		iRegIndex=ADC_50_Hz(ADC1, ADC_Channel_4);
		break;

		case 3005:
		iRegIndex=ADC_50_Hz(ADC1, ADC_Channel_5);
		break;

		case 3006:
		iRegIndex=ADC_50_Hz(ADC1, ADC_Channel_6);
		break;

		case 3007:
		iRegIndex=ADC_50_Hz(ADC1, ADC_Channel_7);
		break;


		case 3008:
		iRegIndex=ADC_50_Hz(ADC1, ADC_Channel_14);
		break;


		case 3009:
		iRegIndex=ADC_50_Hz(ADC1, ADC_Channel_15);
		break;


		case 3010:
		iRegIndex=ADC_50_Hz(ADC1, ADC_Channel_8);
		break;


		case 3011:
		iRegIndex=ADC_50_Hz(ADC1, ADC_Channel_9);
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
{
    return MB_ENOREG;
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
