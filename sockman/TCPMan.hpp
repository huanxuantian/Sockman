
#ifndef _TCP_MAN_HPP_

#define _TCP_MAN_HPP_

#include "CPPSocket/Socket.hpp"
#include "./log4z/log4z.h"

using namespace zsummer::log4z;
using namespace std;

#ifdef WINDOWS

#else
	#include <pthread.h>
	#define USE_PTHREAD
#endif
LoggerId LOGID_TEST;;

#define LOG_TT( log ) LOG_TRACE(LOGID_TEST, log )
#define LOG_TD( log ) LOG_DEBUG(LOGID_TEST, log )
#define LOG_TI( log ) LOG_INFO(LOGID_TEST, log )
#define LOG_TW( log ) LOG_WARN(LOGID_TEST, log )
#define LOG_TE( log ) LOG_ERROR(LOGID_TEST, log )
#define LOG_TA( log ) LOG_ALARM(LOGID_TEST, log )
#define LOG_TF( log ) LOG_FATAL(LOGID_TEST, log )

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

namespace Socket
{
	#ifdef WINDOWS
	typedef void (CALLBACK* DEALPROC)(SocketId s, int ServerPort, const char *ClientIP);
	#else
	typedef void (* DEALPROC)(SocketId s, int ServerPort, const char *ClientIP);
	#endif

	class TCP_MAN : public TCP
	{
    private:
        Address _address;
	private:
		#ifdef WINDOWS
			HANDLE	m_hServerThread;
		#else
			pthread_t thread_listen;
		#endif
		unsigned long int thread_id;
    public:
		TCP_MAN(void);
		TCP_MAN(const TCP_MAN&);
		TCP_MAN accept_client(void);
		bool creat_server(Port, unsigned int);
		bool start_server();
        void send_file(string);
        void receive_file(string);
	};
}
#include "TCPMan.cpp"

#endif
