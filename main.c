//******************************* Include Files ***********************************
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <pthread.h>
#include "UART_Response_Builder.h"
#include "UART_Response_Parser.h"

uint8_t VERSION_VALUE[]     = "FW100";
uint16_t DEVICE_ID_VALUE    = 1000;
uint8_t DEVICE_NAME_VALUE[] = "PC";

typedef enum _eErrorCode{
    TRANSMIT_OK = 0,
    INVALID_DATA_PACKET,
    TRANSMIT_FAILED,
    SET_PARAMETER_ERROR
} eErrorCode;

typedef enum _eHeaderType{
    GET_COMMAND = 1,
    SET_COMMAND
} eHeaderType;

typedef enum _eParamType{
	VERSION = 1,
	DEVICE_ID,
	DEVICE_NAME,
	INVALID_PARAM
} eParamType;

typedef enum _eType{
    TYPE_INT = 1,
    TYPE_CHAR
}eType;

uint8_t GetParamType(uint8_t ucParameter);
bool SetParamValue(uint8_t ucParrameterId, uint8_t pucValueBuffer[], uint8_t ucParamLength);
void GetParameterValue(uint8_t ucParameter, uint8_t pucValueBuffer[], uint8_t *ucParamType, uint8_t *ucLength);
eErrorCode TransmitResponcePacket(_sPacketData *pucPacketData, HANDLE hPort);

void *ServerThread(void *arg)
{
    HANDLE hPort = (HANDLE)arg;
    while (1)
    {
        DWORD dwBytesRead;
        uint8_t readBuffer[100];
        ReadFile(hPort, readBuffer, sizeof(readBuffer) - 1, &dwBytesRead, NULL);

        if (dwBytesRead > 0)
        {
            printf("Received data: \n");
            for (DWORD i = 0; i < dwBytesRead; i++)
            {
                printf("%02x ", (unsigned char)readBuffer[i]); // print each byte in hex
            }
            printf("\n");

             _sPacketData sData;
            
            if(ParsePacket(readBuffer, &sData))
            {
                TransmitResponcePacket(&sData, hPort);
            }
            else
            {
                printf("ERROR: %d Packet parsing failed\n", __LINE__);
            }
        }
    }
}

eErrorCode TransmitResponcePacket(_sPacketData *pucPacketData, HANDLE hPort)
{
    eErrorCode eError = 0;
    eHeaderType eHeader = pucPacketData->ucRequestType;

    if(eHeader == GET_COMMAND)
    {
        uint8_t ucCount = 0;
        uint8_t ucLength = 0;
        _sResponseValue sResponseValue[pucPacketData->ucNumberOfTLVs];

        while(ucCount < pucPacketData->ucNumberOfTLVs)
        {
            eParamType eParam = pucPacketData->psTlv[ucCount].psTlvParam.ucValueBuffer[0];

            if(eParam == INVALID_PARAM)
            {
                eError = INVALID_DATA_PACKET;
            }
            else
            {
                sResponseValue[ucCount].ucParamId = eParam;
                uint8_t ucType;
                GetParameterValue(eParam, sResponseValue[ucCount].ucValueBuffer, &ucType, &ucLength);
                sResponseValue[ucCount].ucResponseType = ucType;
            }
            ucCount++;
        }

        uint8_t ucResponceBuffer[20];

        CreateGetResponsePacket(&sResponseValue, ucCount, ucLength, ucResponceBuffer);
        
        //transmit data
        DWORD dwBytesWritten;

        if(WriteFile(hPort, ucResponceBuffer, sizeof(ucResponceBuffer), &dwBytesWritten, NULL))
        {
            printf("Transmitted data:\n");

            for (DWORD i = 0; i < ucResponceBuffer[1]+2; i++)
            {
                printf("%02x ", (unsigned char)ucResponceBuffer[i]); // print each byte in hex
            }
            printf("\n");
        }
        
    }
    else if(eHeader == SET_COMMAND)
    {
        eParamType eParam = pucPacketData->psTlv[0].psTlvParam.ucType;
        uint8_t ucValueBuffer[20];
        uint8_t ucValueType;

        memcpy(ucValueBuffer, pucPacketData->psTlv[1].psTlvParam.ucValueBuffer, pucPacketData->psTlv[1].psTlvParam.ucLength);
        ucValueType = pucPacketData->psTlv[1].psTlvParam.ucType;

        if(!SetParamValue(eParam, ucValueBuffer, pucPacketData->psTlv[1].psTlvParam.ucLength))
        {
            eError = SET_PARAMETER_ERROR;
        }
         
        CreateSetResponsePacket(eParam, ucValueBuffer);
        DWORD dwBytesWritten;;
        if(WriteFile(hPort, ucValueBuffer, sizeof(ucValueBuffer), &dwBytesWritten, NULL))
        {
            printf("Transmitting data:\n");

            for (DWORD i = 0; i < ucValueBuffer[1]+2; i++)
            {
                printf("%02x ", (unsigned char)ucValueBuffer[i]); // print each byte in hex
            }
            printf("\n");
        }
    }
    else
    {
        eError = INVALID_DATA_PACKET;
    }

    return eError;
}

void GetParameterValue(uint8_t ucParameter, uint8_t pucValueBuffer[], uint8_t *ucParamType, uint8_t *ucLength)
{
    switch (ucParameter)
    {
    case 1:
        memcpy(pucValueBuffer, VERSION_VALUE, strlen(VERSION_VALUE));
        *ucParamType = GetParamType(ucParameter);
        *ucLength = strlen(VERSION_VALUE);
        break;
    case 2:
        memcpy(pucValueBuffer, &DEVICE_ID_VALUE, 4);
        *ucParamType = GetParamType(ucParameter);
        *ucLength = 4;
        break;
    case 3:
        memcpy(pucValueBuffer, DEVICE_NAME_VALUE, strlen(DEVICE_NAME_VALUE));
        *ucParamType = GetParamType(ucParameter);
        *ucLength = strlen(DEVICE_NAME_VALUE);
        break;
    }
}

uint8_t GetParamType(uint8_t ucParameter)
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

bool SetParamValue(uint8_t ucParrameterId, uint8_t pucValueBuffer[], uint8_t ucParamLength)
{
    bool bSetStatus = false;
    switch (ucParrameterId)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:{
        uint8_t ucTemp[10];
        memcpy(ucTemp, DEVICE_NAME_VALUE, strlen(DEVICE_NAME_VALUE));
        memset(DEVICE_NAME_VALUE, 0, strlen(DEVICE_NAME_VALUE));
        memcpy(DEVICE_NAME_VALUE, pucValueBuffer, ucParamLength);
        if(strcmp(DEVICE_NAME_VALUE, ucTemp) != 0)
        {
            bSetStatus = true;
        }
        break;
    }
    default:
        break;
    }

    return bSetStatus;
}

int main()
{
    pthread_t thread_id; 
    HANDLE hPort = CreateFileA("\\\\.\\COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if(hPort == INVALID_HANDLE_VALUE)
    {
        printf("ERROR: %d Failed to open com port\n", __LINE__);
        return 1;
    }

    DCB dcbSerialPorts ={0};

    if(!GetCommState(hPort, &dcbSerialPorts))
    {
        printf("ERROR: %d Failed to get com port state\n", __LINE__);
        return 1;
    }

    dcbSerialPorts.BaudRate = CBR_9600;     //  baud rate
    dcbSerialPorts.ByteSize = 8;            // 8 data bits
    dcbSerialPorts.Parity = NOPARITY;       // No parity
    dcbSerialPorts.StopBits = ONESTOPBIT;   // 1 stop bit

    if(!SetCommState(hPort, &dcbSerialPorts))
    {
        printf("ERROR: %d Failed to set com port state\n", __LINE__);
        return 1;
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (SetCommTimeouts(hPort, &timeouts) == FALSE)
    {
        printf("ERROR: %d Failed to set com timeouts\n", __LINE__);
        return 1;
    }

    uint8_t ucResult = pthread_create(&thread_id, NULL, ServerThread, (void*)hPort);
    if (ucResult) {
        printf("ERROR: return code from pthread_create() is %d\n", ucResult);
        return 1;
    }

    pthread_join(thread_id, NULL);

    CloseHandle(hPort);
    return 0;
}
