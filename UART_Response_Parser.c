//******************************** UART_RESPONSE_PARSER ****************************************
// Copyright (c) 2026
// All Rights Reserved
//*********************************************************************************
//
// File		: UART_Response_Parser.c
// Summary	: UART response parser functions
// Note		: None
// Author	: Abhishek
// Date		: 01-01-2026
//
//*********************************************************************************

//******************************* Include Files ***********************************
#include "UART_Response_Parser.h"
#include "UART_Response_Builder.h"
//******************************* Local Types *************************************

//***************************** Local Constants ***********************************


//***************************** Extern Variables **********************************

//**************************** Global Variables ***********************************


//****************************** Local Functions **********************************

//***************************** Function Definitions ******************************
//******************************.FUNCTION_HEADER.****************************** 
//Purpose : To parse packet 
//Inputs  : ucUARTPacket - Received UART packet
//Outputs : psData - structure holding parsed data 
//Return  : true if successful, false otherwise 
//**********************************************************************************
bool ParsePacket(uint8_t ucUARTPacket[], _sPacketData *psData)
{
	psData->ucRequestType = ucUARTPacket[0];
	psData->ucLength = ucUARTPacket[1];

    if(psData->ucRequestType == 1)
    {
	    psData->ucNumberOfTLVs = GetTLVCount(ucUARTPacket);
    }
    else if(psData->ucRequestType == 2)
    {
        psData->ucNumberOfTLVs = 2;
    }

	uint8_t ucIndex = 2;
	uint8_t ucCount = 0;

	while(ucIndex < psData->ucLength && ucCount < psData->ucNumberOfTLVs)
	{
		psData->psTlv[ucCount].ucTLVNumber = ucCount;
		psData->psTlv[ucCount].psTlvParam.ucType = ucUARTPacket[ucIndex];
		ucIndex++;
        
		psData->psTlv[ucCount].psTlvParam.ucLength = ucUARTPacket[ucIndex];
		ucIndex++;
		if(psData->psTlv[ucCount].psTlvParam.ucLength == 0)
		{
			printf("ERROR: %d Invalid TLV Length\n", __LINE__);
			return false;
		}
		else
		{
			uint8_t ucItration;
			for(ucItration = 0; ucItration < psData->psTlv[ucCount].psTlvParam.ucLength; ucItration++ )
			{
				psData->psTlv[ucCount].psTlvParam.ucValueBuffer[ucItration] = ucUARTPacket[ucIndex+ucItration];
			}
			ucIndex += psData->psTlv[ucCount].psTlvParam.ucLength;
		}

		if(ucIndex > psData->ucLength + 2)// since index start from 2
		{
			return false;
		}

		ucCount++;
	}
	return true;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : To get TLV count 
//Inputs  : ucUARTPacket - Received UART packet
//Return  : none
//
uint8_t GetTLVCount(uint8_t ucUARTPacket[])
{
	uint8_t ucCount = 0;
	uint8_t ucIndex = 1;
	uint8_t ucTotalLength = ucUARTPacket[ucIndex];
	if( ucTotalLength !=0)
	{
		while(ucIndex < ucTotalLength + 2)
		{
			ucIndex ++;		// Since first element is tag
			uint8_t ucLength = ucUARTPacket[ucIndex];
			if(ucLength == 0)
			{
				printf("ERROR: %d Invalid TLV Length\n", __LINE__);
				return 0;		// Invalid TLV
			}
			else
			{
				ucIndex += ucLength;
			}

            ucCount ++;

			if(ucIndex > ucTotalLength+2)
			{
				return ucCount;	
			}
		}
	}
	else
	{
		printf("ERROR: %d Invalid TLV packet\n", __LINE__);
		return 0;	// Invalid TLV
	}

	return ucCount;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : To parse TLV 
//Inputs  : psTLVBuffer - Structure holding TLV data
//Outputs : psTLVData - structure holding parsed TLV data 
//Return  : None 
//
void ParseTLV(uint8_t *psTLVBuffer, _sTLVParams *psTLVData)
{
	uint8_t ucIndex = 0;
	psTLVData->ucType = psTLVBuffer[ucIndex];
	ucIndex++;

	psTLVData->ucLength = psTLVBuffer[ucIndex];
	ucIndex++;

	uint8_t ucItrator;
	uint8_t ucValueBuffer[psTLVData->ucLength];
	for(ucItrator = 0; ucItrator < psTLVData->ucLength; ucItrator++)
	{
		ucValueBuffer[ucItrator] = psTLVBuffer[ucIndex + ucItrator];
	}

	memcpy(psTLVData->ucValueBuffer, ucValueBuffer, psTLVData->ucLength);
	ucIndex += psTLVData->ucLength;
}