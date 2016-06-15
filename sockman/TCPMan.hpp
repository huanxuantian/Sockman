
#ifndef _TCP_MAN_HPP_

#define _TCP_MAN_HPP_

#include "CPPSocket/Socket.hpp"

using namespace std;

#ifdef WINDOWS

#else
	#include <pthread.h>
	#define USE_PTHREAD
#endif

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

	};
}
#include "TCPMan.cpp"

#endif
