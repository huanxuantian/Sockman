#ifndef _LOG_DEBUG_H
#define	_LOG_DEBUG_H



#ifdef __cplusplus
extern "C" {
#endif
#define __packed
#pragma pack(1) 

#pragma pack()

#include <afxmt.h>

static CCriticalSection s_csPRF;

BOOL init_print_thread();
void msg_printf(char *fmt ,...);

#define hex_data_printf(buf,len)	{\
	msg_printf("%s,%d\r\n",__FUNCTION__,__LINE__);\
	s_csPRF.Lock();\
	hex_data_printf_cmp(buf,len);\
    s_csPRF.Unlock();\
}



void hex_data_printf_cmp(unsigned char* buf,int buf_len);


#ifdef __cplusplus
}
#endif



#endif