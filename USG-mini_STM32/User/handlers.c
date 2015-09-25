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

/**********************************************************
 * USART1 interrupt request handler: on reception of a 
 * character 't', toggle LED and transmit a character 'T'
 *********************************************************/
void USART1_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET) // If interrupt by USART_IT_RXNE
    {
      if (USART_GetFlagStatus(USART1, USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE|USART_FLAG_ORE)) //If no errors
      {                   
        RX_Buffer.Buffer[RX_Buffer.Pointer++] = (uint8_t) (USART_ReceiveData(USART2)& 0xFF); // Place data to buffer
				RX_Buffer.Size++;
				RX_Buffer.Pointer %= sizeof(RX_Buffer.Buffer);
      }
      else USART_ReceiveData(USART1); //Receive errors handler 
    }
     
    /* ------------------------------------------------------------ */
    /* Other USART1 interrupts handler can go here ...              */
}
