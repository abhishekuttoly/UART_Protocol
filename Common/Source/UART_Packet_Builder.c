//******************************** UART_RESPONSE_BUILDER ****************************************
// Copyright (c) 2026
// All Rights Reserved
//*********************************************************************************
//
// File		: UART_Response_Builder.c
// Summary	: UART response builder functions
// Note		: None
// Author	: Abhishek
// Date		: 01-01-2026
//
//*********************************************************************************

//******************************* Include Files ***********************************
#include "UART_Packet_Builder.h"

//******************************* Local Types *************************************

//***************************** Local Constants ***********************************


//***************************** Extern Variables **********************************

//**************************** Global Variables ***********************************


//****************************** Local Functions **********************************

//***************************** Function Definitions ******************************
//******************************.FUNCTION_HEADER.****************************** 
//Purpose : To create Get Response Packet
//Inputs  : ucParamId - Parameter id
//Outputs : ucTLVGetPacketBuffer - Response Packet Buffer
//Return  : void
//**********************************************************************************
void UARTPacketBuilderCreateGetPacket(uint8_t ucParamId, uint8_t ucTLVGetPacketBuffer[])
{
	_sTLVParams psTLVParam;
	psTLVParam.ucType = UART_TYPE_PARAMETER;
	psTLVParam.ucLength = UART_VALUE_LENGTH;
	psTLVParam.ucValueBuffer[0] = ucParamId;
	uint8_t ucLengthOfPacket = 0;
	UARTPacketBuilderCreateTLVBuffer(&psTLVParam, ucTLVGetPacketBuffer+2, &ucLengthOfPacket);
	ucTLVGetPacketBuffer[0] = UART_GET_COMMAND;
	ucTLVGetPacketBuffer[1] = ucLengthOfPacket;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : To create Set Response Packet
//Inputs  : ucParamid - Parameter ID
//Inputs  : ucParameterBuffer - Parameter value
//Inputs  : ucParamLength - parameter length
//Outputs : ucTLVSetPacketBuffer - Response Packet Buffer
//Return  : void
//**********************************************************************************
void UARTPacketBuilderCreateSetPacket(uint8_t ucParamId, uint8_t ucParameterBuffer[], uint8_t ucParamLength, uint8_t ucTLVSetPacketBuffer[])
{
	_sTLVParams psTLVParam;
	psTLVParam.ucType = UART_TYPE_PARAMETER;
	psTLVParam.ucLength = UART_VALUE_LENGTH;
	psTLVParam.ucValueBuffer[0]=ucParamId;
	uint8_t ucLengthOfPacket = 0;
	uint8_t ucTLVLength = 0;

	UARTPacketBuilderCreateTLVBuffer(&psTLVParam, ucTLVSetPacketBuffer+2, &ucTLVLength);

	ucLengthOfPacket += ucTLVLength;

	_sTLVParams psSetParamTLV;
	psSetParamTLV.ucType = UART_TYPE_CHAR;
	psSetParamTLV.ucLength = ucParamLength;
	memcpy(psSetParamTLV.ucValueBuffer, ucParameterBuffer, psSetParamTLV.ucLength);
	ucTLVLength = 0;
	UARTPacketBuilderCreateTLVBuffer(&psSetParamTLV, ucTLVSetPacketBuffer+(ucLengthOfPacket+2), &ucTLVLength);
	ucLengthOfPacket += ucTLVLength;

	ucTLVSetPacketBuffer[0] = UART_SET_COMMAND;
	ucTLVSetPacketBuffer[1] = ucLengthOfPacket;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose : To create TLV
//Inputs  : psTLVParam - Pointer to TLV Parameter Structure
//Outputs : ucTLVBuffer - TLV Buffer
//Outputs : ucLength - Length of the TLV
//Return  : void
//**********************************************************************************
void UARTPacketBuilderCreateTLVBuffer(_sTLVParams *psTLVParam, uint8_t ucTLVBuffer[], uint8_t *ucLength)
{
	uint8_t ucIndex = 0;
	ucTLVBuffer[0] = psTLVParam->ucType;
	ucIndex ++;
	ucTLVBuffer[1] = psTLVParam->ucLength;
	ucIndex ++;
	if(psTLVParam->ucLength != 1){
	memcpy(ucTLVBuffer+ucIndex, psTLVParam->ucValueBuffer, psTLVParam->ucLength);
	}
	else{
		ucTLVBuffer[ucIndex] = psTLVParam->ucValueBuffer[0]; // since the length is 1 , it  assign to the buffer.
	}
	ucIndex += psTLVParam->ucLength;
	*ucLength += ucIndex;
}