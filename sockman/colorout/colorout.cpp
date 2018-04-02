#include "stdafx.h"
#include <stdio.h>  
#include <windows.h>  

#include "colorout.h"

  
void LOG(int iLogType, char* szFmt, ...)  
{  
    va_list                     argPtr;  
    char                        szOut[1024*2];  
    DWORD                       dwWs;  
    HANDLE                      hConsoleOutput;  
    CONSOLE_SCREEN_BUFFER_INFO  screenBufferInfo;  
    WORD                        wAttributes;  
      
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);  
    GetConsoleScreenBufferInfo(hConsoleOutput, &screenBufferInfo);  
  
    switch( iLogType )  
    {  
    case LOG_TYPE_NORMAL:       //
        wAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;  
        break;  
    case LOG_TYPE_WARNING:      // 
        wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;  
        break;  
    case LOG_TYPE_ERROR:        //
        wAttributes = FOREGROUND_RED|FOREGROUND_INTENSITY;  
        break;  
    default:  
        wAttributes = screenBufferInfo.wAttributes;  
    }  
      
    va_start(argPtr, szFmt);  
    vsprintf(szOut, szFmt, argPtr);  
    va_end(argPtr);  
  
    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
	//printf(szOut);
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), szOut, strlen(szOut), &dwWs, NULL);  
    SetConsoleTextAttribute(hConsoleOutput, screenBufferInfo.wAttributes);  
}  