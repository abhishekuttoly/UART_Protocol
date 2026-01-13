//******************************** UART_RESPONSE_BUILDER ****************************************
// Copyright (c) 2026
// All Rights Reserved
//*********************************************************************************
//
// File		: UART_Communication.c
// Summary	: UART communication related functions
// Note		: None
// Author	: Abhishek
// Date		: 12-01-2026
//
//*********************************************************************************

//******************************* Include Files ***********************************
#include "UART_Communication.h"

//******************************* Local Types *************************************

//***************************** Local Constants ***********************************


//***************************** Extern Variables **********************************

//**************************** Global Variables ***********************************


//****************************** Local Functions **********************************

//***************************** Function Definitions ******************************
//******************************.FUNCTION_HEADER.****************************** 
//Purpose : Wrapper function for function UART transmit
//Inputs  : hPort - HANDLE object
//Inputs  : ucTXBuffer - buffer to transmit
//Inputs  : ucBufferLength - length of transmitting buffer
//Outputs : ucBytesWritten - Number of bytes written	
//Return  : True if succes, else false
//**********************************************************************************
bool UARTTransmit(HANDLE hPort, uint8_t *ucTXBuffer, uint8_t ucBufferLength, uint8_t *ucBytesWritten)
{
	bool bTransmitStatus = false;
	DWORD dwBytesWritten = 0;
	if (WriteFile(hPort, ucTXBuffer, ucBufferLength, &dwBytesWritten, NULL))
	{
		*ucBytesWritten = dwBytesWritten;
		bTransmitStatus = true;
	}

	return bTransmitStatus;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : Wrapper function for function UART receive
//Inputs  : hPort - HANDLE object
//Inputs  : ucRXBuffer - buffer to transmit
//Inputs  : ucSizeOfBuffer - length of transmitting buffer
//Outputs : ucBytesRead - Number of bytes written	
//Return  : True if succes, else false
//**********************************************************************************
bool UARTRecieve(HANDLE hPort, uint8_t ucRXBuffer[], uint8_t ucSizeOfBuffer, uint8_t *ucBytesRead)
{
	bool bRecieveStatus = false;
	DWORD dwBytesRead = 0;
	if(ReadFile(hPort, ucRXBuffer, ucSizeOfBuffer - 1, &dwBytesRead, NULL))
	{
		*ucBytesRead = dwBytesRead;
		bRecieveStatus = true;
	}
	return bRecieveStatus;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : To initialize comm port settings
//Inputs  : hPort - HANDLE object
//Outputs : None	
//Return  : True if succes, else false
//**********************************************************************************
bool UARTInit(HANDLE hPort)
{
    if(hPort == INVALID_HANDLE_VALUE)
    {
        LOGGER(LOG_LEVEL_ERROR, __FILE__, __LINE__, "Failed to open com port");
        return false;
    }

    DCB dcbSerialPorts ={0};

    if(!GetCommState(hPort, &dcbSerialPorts))
    {
        LOGGER(LOG_LEVEL_ERROR, __FILE__, __LINE__, "Failed to get com port state");
        return false;
    }

    dcbSerialPorts.BaudRate = CBR_9600;     //  baud rate
    dcbSerialPorts.ByteSize = 8;            // 8 data bits
    dcbSerialPorts.Parity = NOPARITY;       // No parity
    dcbSerialPorts.StopBits = ONESTOPBIT;   // 1 stop bit

    if(!SetCommState(hPort, &dcbSerialPorts))
    {
        LOGGER(LOG_LEVEL_ERROR, __FILE__, __LINE__, "Failed to set com port state");
        return false;
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (SetCommTimeouts(hPort, &timeouts) == FALSE)
    {
        LOGGER(LOG_LEVEL_ERROR, __FILE__, __LINE__, "Failed to set com timeouts");
        return false;
    }

    return true;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : To close COMM port
//Inputs  : hPort - HANDLE object
//Outputs : None
//Return  : True if success, else false
//**********************************************************************************
bool UARTCloseCommPort(HANDLE hPort)
{
    CloseHandle(hPort);
    return true;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : To initialize Queue
//Inputs  : pQueue - pointer to the queue
//Outputs : None
//Return  : Void
//**********************************************************************************
void InitQueue(_sQueue *pQueue) {
	pQueue->ucFront = 0;
	pQueue->ucBack = 0;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : Function to check is queue is empty
//Inputs  : pQueue - pointer to the queue
//Outputs : None
//Return  : True if empty, else false
//**********************************************************************************
bool IsEmpty(_sQueue *pQueue)
{
	if (pQueue->ucCount == 0) {
		return true;
	}
	return false;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : Function to check is queue fully occupied
//Inputs  : pQueue - pointer to the queue
//Outputs : None
//Return  : True if full, else false
//**********************************************************************************
bool IsFull(_sQueue *pQueue)
{
	if (pQueue->ucCount == QUEUE_SIZE) {
		return true;
	}
	return false;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : Function to push buffer to queue
//Inputs  : pQueue - pointer to the queue
//Inputs  : ucPushBuffer - buffer to push
//Inputs  : ucLength - length of buffer
//Outputs : None
//Return  : True if success, else false
//**********************************************************************************
bool PushBufferToQueue(_sQueue *pQueue, uint8_t ucPushBuffer[], uint8_t ucLength)
{
	if (IsFull(pQueue)) {
		LOGGER(LOG_LEVEL_ERROR, __FILE__, __LINE__, "Failed to push, since the queue is full");
		return false;
	}

    uint8_t ucIndex;
	for (ucIndex = 0; ucIndex < ucLength; ucIndex++) {
		pQueue->pBuffer[pQueue->ucBack][ucIndex] = ucPushBuffer[ucIndex];
	}

	pQueue->ucLength[pQueue->ucBack] = ucLength;
	pQueue->ucBack = (pQueue->ucBack + 1) % QUEUE_SIZE;
	pQueue->ucCount++;

	return true;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : Function to pop buffer from the queue
//Inputs  : pQueue - pointer to the queue
//Outputs : ucPopBuffer - buffer to store the poped queue
//Outputs : ucLength - length of poped buffer
//Return  : Ture if success, else false
//**********************************************************************************
bool PopFromQueue(_sQueue *pQueue, uint8_t ucPopBuffer[], uint8_t *ucLength)
{
	*ucLength = pQueue->ucLength[pQueue->ucFront];
	uint8_t ucIndex;

	for (ucIndex = 0; ucIndex < *ucLength; ucIndex++) {
		ucPopBuffer[ucIndex] = pQueue->pBuffer[pQueue->ucFront][ucIndex];
	}

	pQueue->ucFront = (pQueue->ucFront + 1) % QUEUE_SIZE;
	pQueue->ucCount--;

	return true;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : Task to create packet buffer and push to queue
//Inputs  : None
//Outputs : None
//Return  : None
//**********************************************************************************
DWORD WINAPI UARTPollingTask(LPVOID lpParam)
{
	_sPollingParameters	sVersionParams		= {1, 60, 0};
	_sPollingParameters	sDeviceIdParams		= { 2, 300, 0};
	_sPollingParameters	sDeviceNameParams	= { 3, 180, 0};

    sDeviceIdParams.ulLastPollingTime = time(NULL);
    sDeviceNameParams.ulLastPollingTime = time(NULL);

	uint8_t ucVersionGetPacketBuffer[64];
	uint8_t ucDeviceIdGetPacketBuffer[64];
	uint8_t ucDeviceNameGetPacketBuffer[64];

	time_t xCurrentTime;

	InitQueue(&pBufferQueue);
	bool bSet = true;	//variable used to set param testing
	bool bSetPC = true;

	/* Infinite loop */
	for (;;) {
		xCurrentTime = time(NULL);
		//set command testing code
		// if(bSet && unTimeinSeconds >= 330){
		// uint8_t ucParamId = 3;
		// uint8_t ucParameterBuffer[] = {"SET"};
		// uint8_t ucParamLength = strlen(ucParameterBuffer);
		// uint8_t ucTLVSetPacketBuffer[64] = {0};
		// CreateSetPacket(ucParamId, ucParameterBuffer,ucParamLength, ucTLVSetPacketBuffer);
		// PushBufferToQueue(&pBufferQueue
		// 		, ucTLVSetPacketBuffer, ucTLVSetPacketBuffer[1]+2);
		// bSet = false;
		// }

		// if (bSetPC && unTimeinSeconds >= 600) {
		// 	uint8_t ucParamId = 3;
		// 	uint8_t ucParameterBuffer[] = { "PC" };
		// 	uint8_t ucParamLength = strlen(ucParameterBuffer);
		// 	uint8_t ucTLVSetPacketBuffer[64] = { 0 };
		// 	CreateSetPacket(ucParamId, ucParameterBuffer, ucParamLength,
		// 			ucTLVSetPacketBuffer);
		// 	PushBufferToQueue(&pBufferQueue, ucTLVSetPacketBuffer,
		// 			ucTLVSetPacketBuffer[1] + 2);
		// 	bSetPC = false;
		// }


		if (sVersionParams.ulLastPollingTime == 0
				&& (xCurrentTime - sVersionParams.ulLastPollingTime  ) > sVersionParams.unPollingInterval) {
            UARTPacketBuilderCreateGetPacket(sVersionParams.ucParamId, ucVersionGetPacketBuffer);
			PushBufferToQueue(&pBufferQueue, ucVersionGetPacketBuffer, 5);
			sVersionParams.ulLastPollingTime = xCurrentTime;
		}

		if ((xCurrentTime - sDeviceIdParams.ulLastPollingTime  )
				>= sDeviceIdParams.unPollingInterval) {
			UARTPacketBuilderCreateGetPacket(sDeviceIdParams.ucParamId, ucDeviceIdGetPacketBuffer);
			PushBufferToQueue(&pBufferQueue, ucDeviceIdGetPacketBuffer, 5);
			sDeviceIdParams.ulLastPollingTime = xCurrentTime;
		}

		if ((xCurrentTime - sDeviceNameParams.ulLastPollingTime)
				>= sDeviceNameParams.unPollingInterval) {
			UARTPacketBuilderCreateGetPacket(sDeviceNameParams.ucParamId,ucDeviceNameGetPacketBuffer);
			PushBufferToQueue(&pBufferQueue, ucDeviceNameGetPacketBuffer, 5);
			sDeviceNameParams.ulLastPollingTime = xCurrentTime;
			}
		Sleep(5000);
	}
	return 0;
}

//******************************.FUNCTION_HEADER.****************************** 
//Purpose : Task to send and receive UART packets
//Inputs  : None
//Outputs : None
//Return  : None
//**********************************************************************************
DWORD WINAPI UARTCommunicationTask(LPVOID lpParam)
{
	HANDLE hPort = (HANDLE)lpParam;

	uint8_t ucRxBuffer[MAX_LENGTH];
    uint8_t ucTxBuffer[MAX_LENGTH];
	uint8_t ucCharValue[MAX_LENGTH];
    uint8_t ucLength = 0;
    _sPacketData sDataPacket;
    DWORD dwBytesWritten = 0;
    DWORD dwBytesRead = 0;
    uint8_t ucTotalTLVs = 0;
    uint8_t ucParameter = 0;
	uint8_t ucIndex = 0;
	uint16_t unIntValue = 0;
	
    /* Infinite loop */
	for (;;) {
		if (!IsEmpty(&pBufferQueue)) {
		

			if (!PopFromQueue(&pBufferQueue, ucTxBuffer, &ucLength)) {
				LOGGER(LOG_LEVEL_ERROR, __FILE__, __LINE__, "Failed to pop buffer from queue");
			}

            // if (WriteFile(hPort, ucTxBuffer, sizeof(ucTxBuffer), &dwBytesWritten, NULL))
            // {
            //     printf("Transmitted data:\n");

            //     for (ucIndex = 0; ucIndex < ucTxBuffer[1] + 2; ucIndex++)
            //     {
            //         printf("%02x ", (unsigned char)ucTxBuffer[ucIndex]); // print each byte in hex
            //     }
            // }
            // else
            // {
            //     LOGGER(LOG_LEVEL_ERROR, __FILE__, __LINE__, "UART transmit failed");
            // }
			uint8_t ucBytesWritten = 0;
			if(UARTTransmit(hPort, ucTxBuffer, sizeof(ucTxBuffer), &ucBytesWritten))
			{
				printf("success\n");
				printf("Transmitted data:\n");

				for (ucIndex = 0; ucIndex < ucTxBuffer[1] + 2; ucIndex++)
				{
					printf("%02x ", (unsigned char)ucTxBuffer[ucIndex]); // print each byte in hex
				}
			}
			else
			{
				LOGGER(LOG_LEVEL_ERROR, __FILE__, __LINE__, "UART transmit failed");
			}

            Sleep(1000);

			uint8_t ucBytesRead = 0;
            // ReadFile(hPort, ucRxBuffer, sizeof(ucRxBuffer) - 1, &dwBytesRead, NULL);
			UARTRecieve(hPort, ucRxBuffer, sizeof(ucRxBuffer), &ucBytesRead);

            if (ucBytesRead > 0)
            {
                printf("\nReceived data: \n");
                for (ucIndex = 0; ucIndex < ucRxBuffer[1]+2; ucIndex++)
                {
                    printf("%02x ", (unsigned char)ucRxBuffer[ucIndex]); // print each byte in hex
                }
                printf("\n");
            

				if (UARTPacketParserParsePacket(ucRxBuffer, &sDataPacket))
				{
                    if (sDataPacket.ucRequestType == 1)
					{
                        if (sDataPacket.ucNumberOfTLVs == 0)
						{
							LOGGER(LOG_LEVEL_ERROR, __FILE__, __LINE__, "Invalid packet");
						}
						else
						{
                            ucTotalTLVs = sDataPacket.ucNumberOfTLVs;

							for (ucIndex = 0; ucIndex < sDataPacket.ucNumberOfTLVs; ucIndex += 2)
							{
								ucParameter = sDataPacket.psTlv[ucIndex+ 1].psTlvParam.ucType;

								if (ucParameter == TYPE_CHAR)
								{
                                    memset(ucCharValue, 0, sizeof(ucCharValue));
									memcpy(ucCharValue,
											sDataPacket.psTlv[ucIndex + 1].psTlvParam.ucValueBuffer,
											sDataPacket.psTlv[ucIndex + 1].psTlvParam.ucLength);

									printf("\nResponse value: ");
									printf("%s\n", ucCharValue);
								}
								else
								{
									memcpy(&unIntValue,
											sDataPacket.psTlv[ucIndex + 1].psTlvParam.ucValueBuffer,
											sDataPacket.psTlv[ucIndex + 1].psTlvParam.ucLength);
									printf("Response value = %d\n", unIntValue);
								}
							}
						}
					}
                    

					if (sDataPacket.ucRequestType == 2) {
						LOGGER(LOG_LEVEL_INFO, __FILE__, __LINE__, "Set Success");
					}
				}
				else
				{
					LOGGER(LOG_LEVEL_ERROR, __FILE__, __LINE__, "Failed to parse the packet");
				}
			}
			else
			{
				LOGGER(LOG_LEVEL_ERROR, __FILE__, __LINE__, "UART Recieve failed due to timeout");
			}
		}
		Sleep(1000);
	}
	return 0;
}

uint8_t UARTGetParamType(uint8_t ucParameter)
{
    uint8_t ucParamType;
	switch(ucParameter)
	{
		case 2:
			ucParamType = TYPE_INT;
			break;
		case 1:
		case 3:
			ucParamType =  TYPE_CHAR;
			break;
        default:
            break;
	}
	return ucParamType;
}