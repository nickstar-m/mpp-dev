/******************************************************************************
 * @file     terminal.h
 * @brief    USG-mini generator main board firmware
 * @version  V1.00
 * @date     29. September 2015
 * @author   Nickstar
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2015 MedPromPrylad

   ---------------------------------------------------------------------------*/
	 
#ifndef TERMINAL_H
#define TERMINAL_H

#include "global.h"

//==================================================
// Proc prototypes
//==================================================
void TerminalProcessor(void);

void Ver(void);
void Ser(void);
void Stat(void);
void Log(void);
	
#define PROMT  "\r\n\r\nUSG-mini>"

//==================================================
// COMMANDS
//==================================================
const char* Commands[] = { "GET @1",
                           "SET @1=@2",
                           "STATUS",
                           "LOG",
                           "VER",
                           "SER",
                         };
const char *(*CmdPtr)[] = &Commands;

#define NUMBER_OF_COMMANDS  6														

//==================================================
// READ ONLY VARIABLES
//==================================================

const char* RO_Variables[] = {
	                            "SERIAL",
															"MODEL",
															"PROD_DATE",
															"REV_FW",
															"REV_HW",
															"PSERIAL",
															"PMODEL",

															"T_CASE",
															"T_KEYS",
															"VIN",
															"V33",
															"V15",
															"HV",
															"CG",
															"CH",
															"UPTIME",
															"HEAT_STATE",
															"TRIAC_ANGLE",
															"GEN_STATE",
															"LAST_ERROR",

															"TIME_ON",
															"TIME_H",
															"TIME_T",
															"S_KWH",

															"RES_FR",
															"CUR_FR",
														 };

const char *(*roVarPtr)[] = &RO_Variables;

//==================================================
// READ/WRITE VARIABLES
//==================================================

const char* RW_Variables[] = {
	                            "TR_N",
															"TR_PMAX",
															"HT_PMAX",
															"FR",
															"FR_BW",
															"US_P",
															"HT_P",
															"HEAT",
															"GEN",
															"SWP_DP",
															"SWP_FR",
															"SWP",
															"PUL_FR",
															"PUL_DC",
															"PUL",

															"FR_S",
															"SAVE_P",
															"T_TRANS_MAX",
															"T_CASE_MAX",
															"VIN_MAX",
															"PROT_FACT",
														 };

const char *(*rwVarPtr)[] = &RW_Variables;

const uint32_t RW_VariableLimits[][2] = {
																					 {1, 20},
																					 {1, 1000},
																					 {10, 2000},
																					 {18000, 125000},
																					 {10, 3000},
																					 {1, 1000},
																					 {10, 2000},
																					 {0, 1},
																					 {0, 1},
																					 {0, 3000},
																					 {0, 100},
																					 {0, 1},
																					 {0, 100},
																					 {1, 100},
																					 {0, 1},
																					 {0, 255},
																					 {1, 3600},
																					 {25, 85},
																					 {25, 85},
																					 {200, 265},
																					 {100, 1000}, //1.00 to 10.0 of the nominal current
                                        };

//==================================================
// COMMAND LINE ERROR MESSAGES
//==================================================
const char* ERR_Mesages[] = { 
	                            "Illegal command",
															"Illegal parameter",
															"Illegal parameter type",
															"Incorrect syntax",
															"Value is less than allowed. Value must be between %d and %d.",
															"Value is larger than allowed. Value must be between %d and %d.",
														};
const char *(*ErrPtr)[] = &ERR_Mesages;

#endif
