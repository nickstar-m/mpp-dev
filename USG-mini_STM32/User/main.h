/******************************************************************************
 * @file     main.h
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
#include "periph_init.h"
#include "utilites.h"
#include "handlers.h"

#define CONST_INIT_DATA {"UM154000001\0", "USG-mini\0", "23/09/2015\0", "FW1.001.A\0", "HW1.001.A\0", "UZM-001\0", "12345678\0"}

void TerminalProcessor(void);

