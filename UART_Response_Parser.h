//**************************** UART_RESPONSE_PARSER *****************************
//  Copyright (c) 2026 
//  All Rights Reserved 
//***************************************************************************** 
// 
// File		: UART_Response_Parser.h
// Summary	: This configurations header file contains structure and function declarations
//		       for UART Response Parser functions
// Note		: None
// Author	: Abhishek
// Date		: 01-01-2026
// 
//***************************************************************************** 
#ifndef _UART_RESPONSE_PARSER_H_ 
#define _UART_RESPONSE_PARSER_H_
//******************************* Include Files ******************************* 
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "UART_Response_Builder.h"
//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 
 
//***************************** Global Structure ******************************
typedef struct __sTLV{
	uint8_t	ucTLVNumber;		// count of value
	_sTLVParams psTlvParam;		// TLV data holding structure
	} _sTLV;

typedef struct __sPacketData{
	uint8_t ucRequestType;			// TLV header
	uint8_t ucLength;				// Length of TLV buffer
	uint8_t ucNumberOfTLVs;			// Number of TLVs
	_sTLV psTlv[];		// structure holding multiple TLV data
	} _sPacketData;
//**************************** Forward Declarations *************************** 
bool ParsePacket(uint8_t ucUARTPacket[], _sPacketData *psData);
uint8_t GetTLVCount(uint8_t ucUARTPacket[]);
void ParseTLV(uint8_t *psTLVBuffer, _sTLVParams *psTLVData);
//***************************** Function Declaration **************************
#endif // _UART_RESPONSE_PARSER_H_ 
// EOF