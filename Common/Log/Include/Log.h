//**************************** LOG *****************************
//  Copyright (c) 2026 
//  All Rights Reserved 
//***************************************************************************** 
// 
// File		: Log.h
// Summary	: This configurations header file contains structure and function declarations
//		       for log functions
// Note		: None
// Author	: Abhishek
// Date		: 13-01-2026
// 
//***************************************************************************** 
#ifndef _LOG_H_ 
#define _LOG_H_
//******************************* Include Files ******************************* 
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 
#define UART_GET_COMMAND 1

//***************************** Global Structure ******************************
typedef enum __eLogLevel{
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
}_eLogLevel;

//**************************** Forward Declarations *************************** 
//***************************** Function Declaration **************************
void LOGGER(_eLogLevel eLogLevel, uint8_t ucFileNameBuffer[], uint16_t unLineNumber, uint8_t ucLogBuffer[]);
#endif // _LOG_H_