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
	 
// -----------------------------------
//  USART BUFFERS
// -----------------------------------

#if (INTERFACE_TYPE == TERMINAL)
  extern char TX_buffer[128];
  extern char RX_buffer[64];
#endif


