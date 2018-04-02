
#include "stdafx.h"
#include "log_debug.h"

#include <queue>
using namespace std;

#ifndef uint8
#define uint8 UINT8
#endif

#ifndef uint16
#define uint16 UINT16
#endif
#define ND_DEBUG_LOG
#define LOG_PATH	"./app.log"
HANDLE      m_hThreadDebug;
/*
extern void SaveLog(char *buf);
extern void SaveLog(char *buf,UINT16 len);
extern void SaveLog_Hex(uint8 *buf,uint16 len);
extern void SaveLog_Hex(char *buf,uint16 len);
*/

void SaveLog(char *buf,uint16 len)
{
	char  path[64] = {0};
	FILE* fp = NULL;
    fp = fopen(LOG_PATH,"a+b");
	if(!fp)  return ;
	fwrite(buf,1,len,fp);
	fclose(fp);
	return;
}

void SaveLog(char *buf)
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	//
	char temp[1024];
	memset(temp,0,sizeof(temp));
	sprintf(temp,"%dƒÍ%d‘¬%d»’ %02d:%02d:%02d %s%s\r\n", 
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		buf, "\0");
	SaveLog(temp,strlen(temp));
}

void SaveLog_Hex(char *buf,uint16 len)
{
	char  path[64] = {0};
	char  tmp[3] = {0};
	FILE* fp = NULL;
	fp = fopen(LOG_PATH,"a+b");
	if(!fp)  return ;
	for (int i=0; i<len; i++) {
		sprintf(tmp,"%02X ",buf[i]);
		fwrite(tmp,1,3,fp);
		if (i % 15 == 0)
		{
			fwrite("\n",1,3,fp);

		}
	}	
	fclose(fp);
	return;
}

void SaveLog_Hex(uint8 *buf,uint16 len)
{
	char  path[64] = {0};
	char  tmp[3] = {0};
	FILE* fp = NULL;
	fp = fopen(LOG_PATH,"a+b");
	if(!fp)  return ;
	for (int i=0; i<len; i++) {
		sprintf(tmp,"%02X ",buf[i]);
		fwrite(tmp,1,3,fp);
		if (i % 15 == 0&&(i))
		{
			fwrite("\r\n",1,2,fp);

		}
	}
	fwrite("\r\n",1,2,fp);
	fclose(fp);
	return;
}



queue<CStringA> Msglist;

BOOL DebugState =FALSE;

DWORD WINAPI vThreadDebugProc(LPVOID lpParameter)
{
	DebugState = TRUE;
	while(1)
	{
		
		if(!Msglist.empty())
		{
			CStringA msg_print =Msglist.front();
			Msglist.pop();
#ifdef ND_DEBUG_LOG
			//SaveLog((char*)msg_print.GetString());
			SaveLog(":");//TIMESTAMP:\r\n
			SaveLog((char*)msg_print.GetString(),(uint16)msg_print.GetLength());
#else
			printf(msg_print.GetString());
#endif
			//printf(msg_print.GetString());
		}
		Sleep(20);
	}
	DebugState = FALSE;
	return 0;
}

BOOL init_print_thread()
{
	if (m_hThreadDebug != NULL) {
		CloseHandle(m_hThreadDebug);
	}

	m_hThreadDebug = ::CreateThread(NULL,0,vThreadDebugProc,NULL,0,NULL);
	/*
	CStringA msg = "2016-12-12 00:00:00";

	hex_data_printf((unsigned char*)msg.GetString(),msg.GetLength());
	*/
	if (NULL == m_hThreadDebug) {	
		return FALSE;
	} else {
		return TRUE;
	}
}


void hex_data_printf_cmp(unsigned char* buf,int buf_len)
{
	CStringA msg;
	int i;
	
	if(DebugState)
	{
		msg.Empty();
		for(i=0;i<buf_len;i++)
		{
			msg.AppendFormat("%02X ",buf[i]);
			//printf("%02X ",buf[i]);
		}
		msg.Append("\r\n");
		Msglist.push(msg);
	}
	else
	{
#ifdef ND_DEBUG_LOG
		SaveLog_Hex(buf,buf_len);
#else
		for(i=0;i<buf_len;i++)
		{
			printf("%02X ",buf[i]);
		}
		printf("\r\n");
#endif
	}

	//
}

void msg_printf(char *fmt ,...)
{
	va_list v_arg;
	char string[1024];
	int len = 0;

	CStringA msg;
	va_start(v_arg,fmt);

	vsprintf(string,fmt,v_arg);
	s_csPRF.Lock();
	//printf(string);
	if(DebugState)
	{
		msg.Empty();
		msg+=string;
		Msglist.push(msg);
	}
	else
	{
#ifdef ND_DEBUG_LOG
		SaveLog(string,strlen(string));
#else
		printf(string);
#endif
	}

	s_csPRF.Unlock();
	va_end(v_arg);
}