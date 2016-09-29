

#ifndef _TCP_MAN_CPP_
#define _TCP_MAN_CPP_

#include "TCPMan.hpp"
namespace Socket
{

	TCP_MAN::TCP_MAN(void)
	{
		CommonSocket(SOCK_STREAM);
	}

	TCP_MAN::TCP_MAN(const TCP_MAN &tcp)
	{
		CommonSocket();
		this->_socket_id = tcp._socket_id;
		this->_opened = tcp._opened;
		this->_binded = tcp._binded;
	}

	TCP_MAN TCP_MAN::accept_client(void)
    {
		TCP_MAN ret;
        socklen_t len = sizeof(struct sockaddr_in);

        ret.close();
        ret._socket_id = accept(this->_socket_id, (struct sockaddr*)&ret._address, &len);
        ret._opened = true;
        ret._binded = true;

        return ret;
    }
}
#endif
