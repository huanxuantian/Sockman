
#include		<stdio.h>
#include		<stdlib.h>
#include "colorout.h"

#ifdef WINDOWS

#include <windows.h>  
#else
#include 	<stdarg.h>
#endif



	#ifdef WINDOWS
	void LOG(int iLogType, char* szFmt, ...)
	{
		va_list                     argPtr;
		char                        szOut[MAX_LOG_LINESIZE];
		DWORD                       dwWs;
		HANDLE                      hConsoleOutput;
		CONSOLE_SCREEN_BUFFER_INFO  screenBufferInfo;
		WORD                        wAttributes;

		hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hConsoleOutput, &screenBufferInfo);

		switch( iLogType )
		{
		case LOG_TYPE_NORMAL:       //
			wAttributes = DBG_C_LIGHT_BLUE;
			break;
		case LOG_TYPE_WARNING:      //
			wAttributes = DBG_C_YELLOW;
			break;
		case LOG_TYPE_ERROR:        //
			wAttributes = DBG_C_LIGHT_RED;
			break;
		case LOG_TYPE_NONE:
			  return;
		default:
			wAttributes = screenBufferInfo.wAttributes;
		}

		va_start(argPtr, szFmt);
		vsprintf(szOut, szFmt, argPtr);
		va_end(argPtr);

		SetConsoleTextAttribute(hConsoleOutput, wAttributes);
		printf(szOut);
		//WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), szOut, strlen(szOut), &dwWs, NULL);
		SetConsoleTextAttribute(hConsoleOutput, screenBufferInfo.wAttributes);
	}
	#else

	void LOG(int iLogType, char* szFmt, ...)
	{
		char strings[MAX_LOG_LINESIZE];
	
		switch( iLogType )
		{
		case LOG_TYPE_NORMAL:       //
			printf(DBG_C_LIGHT_BLUE);
			break;
		case LOG_TYPE_WARNING:      //
			printf(DBG_C_YELLOW);
			break;
		case LOG_TYPE_ERROR:        //
			printf(DBG_C_LIGHT_RED);
			break;
		case LOG_TYPE_NONE:
			  return;
		default:
			printf(DBG_C_NONE);
		}

		va_list args;
		va_start(args, szFmt);
		vsprintf(strings, szFmt, args);
		va_end(args);
		printf("%s", strings);
		printf(DBG_C_NONE);
		fflush(stdout);
	}


#endif
