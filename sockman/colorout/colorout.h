
#pragma once

#define LOG_TYPE_NORMAL 0  
#define LOG_TYPE_WARNING 1  
#define LOG_TYPE_ERROR 2  


void LOG(int iLogType, char* szFmt, ...);