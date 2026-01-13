//******************************** LOG ****************************************
// Copyright (c) 2026
// All Rights Reserved
//*********************************************************************************
//
// File		: Log.c
// Summary	: Log functions
// Note		: None
// Author	: Abhishek
// Date		: 13-01-2026
//
//*********************************************************************************

//******************************* Include Files ***********************************
#include "Log.h"

//******************************* Local Types *************************************

//***************************** Local Constants ***********************************


//***************************** Extern Variables **********************************

//**************************** Global Variables ***********************************


//****************************** Local Functions **********************************

//***************************** Function Definitions ******************************
//******************************.FUNCTION_HEADER.****************************** 
//Purpose : To Log logs
//Inputs  : eLogLevel - Log level
//Inputs  : ucFileNameBuffer - File name buffer
//Inputs  : unLineNumber - Line number
//Inputs  : ucLogBuffer -  error log
//Return  : void
//**********************************************************************************
void LOGGER(_eLogLevel eLogLevel, uint8_t ucFileNameBuffer[], uint16_t unLineNumber, uint8_t ucLogBuffer[])
{
    uint8_t ucLogLevel[20] = "";

    switch(eLogLevel)
    {
        case 1:
        strcpy((char*)ucLogLevel, "ERROR");
        break;
        case 2:
        strcpy((char*)ucLogLevel, "WARNING");
        break;
        case 3:
        strcpy((char*)ucLogLevel, "INFO");
        break;
    }

    printf("%s::%s::%d    %s\n", ucLogLevel, ucFileNameBuffer, unLineNumber, ucLogBuffer);
}
