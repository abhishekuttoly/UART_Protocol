//******************************* Include Files ***********************************
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "UART_Communication.h"
#include "Log.h"

#define FILE_NAME "main.c"

int main()
{
    HANDLE hCommunicationThread;
    HANDLE hPollingThread;
    HANDLE hPort = CreateFileA("\\\\.\\COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if(hPort == INVALID_HANDLE_VALUE)
    {
        LOGGER(LOG_LEVEL_ERROR, FILE_NAME, __LINE__, "Failed to open com port");
        return 1;
    }

    if(!UARTInit(hPort))
    {
        LOGGER(LOG_LEVEL_ERROR, FILE_NAME, __LINE__, "UART initialization failed");
        return 1;
    }

    // Create Communication Thread
    hCommunicationThread = CreateThread(
        NULL,                           // Security attributes
        0,                              // Stack size (0 = default)
        (LPTHREAD_START_ROUTINE)UARTCommunicationTask,
        (LPVOID)hPort,                  // Parameter to pass to thread
        0,                              // Creation flags
        NULL                            // Thread ID output
    );

    if (hCommunicationThread == NULL) {
        LOGGER(LOG_LEVEL_ERROR, FILE_NAME, __LINE__, "Failed to create CommunicationTask thread");
        UARTCloseCommPort(hPort);
        return 1;
    }

    // Create Polling Thread
    hPollingThread = CreateThread(
        NULL,                           // Security attributes
        0,                              // Stack size (0 = default)
        (LPTHREAD_START_ROUTINE)UARTPollingTask,
        NULL,                           // Parameter to pass to thread
        0,                              // Creation flags
        NULL                            // Thread ID output
    );

    if (hPollingThread == NULL) {
        LOGGER(LOG_LEVEL_ERROR, FILE_NAME, __LINE__, "Failed to create PollingTask thread");
        CloseHandle(hCommunicationThread);
        UARTCloseCommPort(hPort);
        return 1;
    }

    // Wait for both threads to complete
    WaitForSingleObject(hCommunicationThread, INFINITE);
    WaitForSingleObject(hPollingThread, INFINITE);

    // Clean up
    CloseHandle(hCommunicationThread);
    CloseHandle(hPollingThread);
    UARTCloseCommPort(hPort);

    return 0;
}
