
#ifndef _LOGER_HPP_

#define _LOGER_HPP_

#include <iostream>
#include "./log4z/log4z.h"

//using namespace zsummer::log4z;
using namespace std;

#ifdef WIN32

#else
	#include <pthread.h>
	#define USE_PTHREAD
#endif

LoggerId get_logger();

#define LOG_TT( log ) LOG_TRACE((LoggerId)get_logger(), log )
#define LOG_TD( log ) LOG_DEBUG((LoggerId)get_logger(), log )
#define LOG_TI( log ) LOG_INFO((LoggerId)get_logger(), log )
#define LOG_TW( log ) LOG_WARN((LoggerId)get_logger(), log )
#define LOG_TE( log ) LOG_ERROR((LoggerId)get_logger(), log )
#define LOG_TA( log ) LOG_ALARM((LoggerId)get_logger(), log )
#define LOG_TF( log ) LOG_FATAL((LoggerId)get_logger(), log )

/*
#define setup_loger() { cout<<"setup loger!!"<<endl; \
	LOGID_TEST = ILog4zManager::getRef().createLogger("test" ); \
	ILog4zManager::getRef().setLoggerDisplay(LOG4Z_MAIN_LOGGER_ID, false); \
	ILog4zManager::getRef().setLoggerOutFile(LOG4Z_MAIN_LOGGER_ID, false); \
	ILog4zManager::getRef().setLoggerDisplay(LOGID_TEST, true); \
	ILog4zManager::getRef().setLoggerOutFile(LOGID_TEST, true); \
    ILog4zManager::getRef().start(); \
    cout<<"LOGID_TEST:"<<LOGID_TEST<<("!setup finish!!")<<endl; \
    }
*/
int setup_loger();
/*
int setup_loger()
{
	//start log4z
	cout<<"setup loger!!"<<endl;
	LOGID_TEST = ILog4zManager::getRef().createLogger("test" );
   // ILog4zManager::getRef().config("config.cfg");
	ILog4zManager::getRef().setLoggerDisplay(LOG4Z_MAIN_LOGGER_ID, false);
	ILog4zManager::getRef().setLoggerOutFile(LOG4Z_MAIN_LOGGER_ID, false);

	ILog4zManager::getRef().setLoggerDisplay(LOGID_TEST, true);
	ILog4zManager::getRef().setLoggerOutFile(LOGID_TEST, true);
    ILog4zManager::getRef().start();
    cout<<"LOGID_TEST:"<<LOGID_TEST<<("!setup finish!!")<<endl;
    return 0;
}
*/

#endif
