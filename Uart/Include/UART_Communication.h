//**************************** UART_COMMUNICATION *****************************
//  Copyright (c) 2026 
//  All Rights Reserved 
//***************************************************************************** 
// 
// File		: UART_Response_Builder.h
// Summary	: This configurations header file contains structure and function declarations
//		       for UART communication
// Note		: None
// Author	: Abhishek
// Date		: 12-01-2026
// 
//***************************************************************************** 
#ifndef _UART_COMMUNICATION_H_ 
#define _UART_COMMUNICATION_H_
//******************************* Include Files ******************************* 
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "UART_Packet_Parser.h"
#include "UART_Packet_Builder.h"
#include "Log.h"
//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 
#define QUEUE_SIZE 10
#define MAX_LENGTH 20

//***************************** Global Structure ******************************
typedef struct __sQueue {
	uint8_t pBuffer[QUEUE_SIZE][MAX_LENGTH];
	uint8_t ucLength[QUEUE_SIZE];
	uint8_t ucFront;
	uint8_t ucBack;
	uint8_t ucCount;
} _sQueue;

typedef struct __sPollingParameters {
	uint8_t ucParamId;				// Parameter id
	uint16_t unPollingInterval;		// Parameter polling interval
	time_t ulLastPollingTime;		// Last polling time
} _sPollingParameters;

typedef enum _eType{
    TYPE_INT = 1,
    TYPE_CHAR
}eType;

_sQueue pBufferQueue;
//**************************** Forward Declarations *************************** 

//***************************** Function Declaration **************************
bool UARTInit(HANDLE hPort);
bool UARTCloseCommPort(HANDLE hPort);
void InitQueue(_sQueue *pQueue);
bool IsEmpty(_sQueue *pQueue);
bool IsFull(_sQueue *pQueue);
bool PushBufferToQueue(_sQueue *pQueue, uint8_t ucPushBuffer[], uint8_t ucLength);
bool PopFromQueue(_sQueue *pQueue, uint8_t ucPopBuffer[], uint8_t *ucLength);
DWORD WINAPI UARTPollingTask(LPVOID lpParam);
DWORD WINAPI UARTCommunicationTask(LPVOID lpParam);
uint8_t UARTGetParamType(uint8_t ucParameter);
#endif // _UART_RESPONSE_BUILDER_H_ 
// EOF