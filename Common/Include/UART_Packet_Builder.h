//**************************** UART_RESPONSE_BUILDER *****************************
//  Copyright (c) 2026 
//  All Rights Reserved 
//***************************************************************************** 
// 
// File		: UART_Response_Builder.h
// Summary	: This configurations header file contains structure and function declarations
//		       for UART Response Builder functions
// Note		: None
// Author	: Abhishek
// Date		: 01-01-2026
// 
//***************************************************************************** 
#ifndef _UART_RESPONSE_BUILDER_H_ 
#define _UART_RESPONSE_BUILDER_H_
//******************************* Include Files ******************************* 
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 
#define UART_GET_COMMAND 1
#define UART_SET_COMMAND 2
#define UART_TYPE_INT 1
#define UART_TYPE_CHAR 2
#define UART_TYPE_PARAMETER 3
#define UART_VALUE_LENGTH 1
//***************************** Global Structure ******************************
typedef struct __sResponseValue{
	uint8_t ucResponseType;
	uint8_t ucParamId;
	uint8_t ucValueBuffer[];
	} _sResponseValue;

typedef struct __sTLVParams{
	uint8_t ucType;
	uint8_t ucLength; 
	uint8_t ucValueBuffer[10];
	} _sTLVParams;

//**************************** Forward Declarations *************************** 
//***************************** Function Declaration **************************
void UARTPacketBuilderCreateGetPacket(uint8_t ucParamId, uint8_t ucTLVGetPacketBuffer[]);
void UARTPacketBuilderCreateSetPacket(uint8_t ucParamId, uint8_t ucParameterBuffer[], uint8_t ucParamLength, uint8_t ucTLVSetPacketBuffer[]);
void UARTPacketBuilderCreateTLVBuffer(_sTLVParams *psTLVParam, uint8_t ucTLVBuffer[], uint8_t *ucLength);
#endif // _UART_RESPONSE_BUILDER_H_ 
// EOF