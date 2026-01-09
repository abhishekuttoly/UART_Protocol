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
void CreateGetResponsePacket(_sResponseValue *psResponseValue, uint8_t ucNumberOfItems, uint8_t ucLengthOfResponseValue, uint8_t pucResponsePacket[]);
void CreateSetResponsePacket(uint8_t ucParameterID, uint8_t pucResponsePacket[]);
uint8_t CreateTLV(_sTLVParams *psTLVParam, uint8_t ucTLVBuffer[]);
#endif // _UART_RESPONSE_BUILDER_H_ 
// EOF