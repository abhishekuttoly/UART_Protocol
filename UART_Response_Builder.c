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
#include "UART_Response_Builder.h"

//******************************* Local Types *************************************

//***************************** Local Constants ***********************************


//***************************** Extern Variables **********************************

//**************************** Global Variables ***********************************


//****************************** Local Functions **********************************

//***************************** Function Definitions ******************************
//******************************.FUNCTION_HEADER.****************************** 
//Purpose : To create Get Response Packet
//Inputs  : psResponseValue - Pointer to Response Value Structure
//Inputs  : ucNumberOfItems - Number of Items in the Response
//Inputs  : ucLengthOfResponseValue - Length of the Response Value
//Outputs : pucResponsePacket - Pointer to Response Packet Buffer
//Return  : void
//**********************************************************************************
void CreateGetResponsePacket(_sResponseValue *psResponseValue, uint8_t ucNumberOfItems, uint8_t ucLengthOfResponseValue, uint8_t pucResponsePacket[])
{
    uint8_t ucIndex = 2; // Start after length bytes
    uint8_t ucCounter = 0;
    uint8_t ucTotalLength = 0;

    while(ucCounter < ucNumberOfItems)
    {
        _sTLVParams sTLVParam;

        sTLVParam.ucType = 1;
        sTLVParam.ucLength = 1;
        sTLVParam.ucValueBuffer[0] = psResponseValue->ucParamId;
        uint8_t ucLength = 0;
        ucLength = CreateTLV(&sTLVParam, pucResponsePacket+ucIndex);

        ucTotalLength += ucLength;

        // TLV holds response data
        _sTLVParams sResponseTLV;
        sResponseTLV.ucType = psResponseValue->ucResponseType;
        sResponseTLV.ucLength = ucLengthOfResponseValue;

        memcpy(sResponseTLV.ucValueBuffer, psResponseValue->ucValueBuffer, sResponseTLV.ucLength);

        ucIndex += ucLength;
        ucLength = 0;
        ucLength = CreateTLV(&sResponseTLV, pucResponsePacket+ucIndex);
        ucIndex += ucLength;
        ucTotalLength += ucLength;
        ucCounter++;
    }
    pucResponsePacket[0] = 1;
    pucResponsePacket[1] = ucTotalLength;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : To create Set Response Packet
//Inputs  : ucParameterID - Parameter ID
//Outputs : pucResponsePacket - Pointer to Response Packet Buffer
//Return  : void
//**********************************************************************************
void CreateSetResponsePacket(uint8_t ucParameterID, uint8_t pucResponsePacket[])
{
    _sTLVParams sTLVParam;

    sTLVParam.ucType = 1;   //param
    sTLVParam.ucLength = 1;
    sTLVParam.ucValueBuffer[0] = ucParameterID;

    uint8_t ucLength = 0;
    ucLength = CreateTLV(&sTLVParam, pucResponsePacket+2);

    pucResponsePacket[0] = 2;   //set
    pucResponsePacket[1] = ucLength;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose : To create TLV
//Inputs  : psTLVParam - Pointer to TLV Parameter Structure
//Outputs : ucTLVBuffer - TLV Buffer
//Outputs : ucLength - Length of the TLV
//Return  : void
//**********************************************************************************
uint8_t CreateTLV(_sTLVParams *psTLVParam, uint8_t ucTLVBuffer[])
{
    uint8_t ucIndex = 0;
    ucTLVBuffer[0] = psTLVParam->ucType;
	ucIndex++;
    ucTLVBuffer[1] = psTLVParam->ucLength;
	ucIndex++;
	memcpy(ucTLVBuffer+ucIndex, psTLVParam->ucValueBuffer, psTLVParam->ucLength);
	ucIndex += psTLVParam->ucLength;
    return ucIndex;
}