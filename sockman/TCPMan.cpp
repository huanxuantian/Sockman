

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
	bool TCP_MAN::creat_server(Port port, unsigned int listeners = 1)
	{
        CommonSocket::listen_on_port(port);

        if (listen(this->_socket_id, listeners) != 0)
        {
            stringstream error;
            error << "[listen_on_port] with [port=" << port << "] [listeners=" << listeners << "] Cannot bind socket";
            throw SocketException(error.str());
            return false;
        }
        return true;
	}
	void TCP_MAN::send_file(string file_name)
	{
	    unsigned long long file_size;
	    unsigned char chunk[SOCKET_MAX_BUFFER_LEN];
	    unsigned char sync;
	    fstream fp(file_name.c_str(), ios::in | ios::binary);

	    if (!fp.is_open())
	    {
	        stringstream error;
	        error << "[sSocketend_file] with [filename=" << file_name << "] Cannot open the file";
	        throw SocketException(error.str());
	    }

	    fp.seekg(0, ifstream::end);
	    file_size = fp.tellg();
	    fp.seekg(0, ifstream::beg);
	    this->send<unsigned long long>(&file_size, 1);
	    LOG_TI(Log4zBinary(&file_size, sizeof(file_size)));//debug 2016-9-29

	    for (unsigned long long i = 0; i < file_size / SOCKET_MAX_BUFFER_LEN; i++)
	    {
	        this->receive<unsigned char>(&sync, 1);
	        fp.read((char*)chunk, SOCKET_MAX_BUFFER_LEN);
	        LOG_TI(Log4zBinary(&chunk, SOCKET_MAX_BUFFER_LEN));//debug 2016-9-29
	        this->send<unsigned char>(chunk, SOCKET_MAX_BUFFER_LEN);
	    }

	    if ((file_size % SOCKET_MAX_BUFFER_LEN) != 0)
	    {
	        this->receive<unsigned char>(&sync, 1);
	        fp.read((char*)chunk, file_size % SOCKET_MAX_BUFFER_LEN);
	        LOG_TI(Log4zBinary(&chunk, file_size % SOCKET_MAX_BUFFER_LEN));//debug 2016-9-29
	        this->send<unsigned char>(chunk, file_size % SOCKET_MAX_BUFFER_LEN);
	    }

	    fp.close();
	}

	void TCP_MAN::receive_file(string file_name)
	{
	    unsigned long long file_size;
	    unsigned char chunk[SOCKET_MAX_BUFFER_LEN];
	    unsigned char sync;
	    fstream fp(file_name.c_str(), ios::out | ios::binary);

	    if (!fp.is_open())
	    {
	        stringstream error;
	        error << "[send_file] with [filename=" << file_name << "] Cannot open the file";
	        throw SocketException(error.str());
	    }

	    this->receive<unsigned long long>(&file_size, 1);
	    LOG_TI(Log4zBinary(&file_size, sizeof(file_size)));//debug 2016-9-29

	    for(unsigned long long i = 0; i < file_size / SOCKET_MAX_BUFFER_LEN; i++)
	    {
	        this->send<unsigned char>(&sync, 1);
	        this->receive<unsigned char>(chunk, SOCKET_MAX_BUFFER_LEN);
	        LOG_TI(Log4zBinary(&chunk, SOCKET_MAX_BUFFER_LEN));//debug 2016-9-29
	        fp.write((char*)chunk, SOCKET_MAX_BUFFER_LEN);
	    }

	    if ((file_size % SOCKET_MAX_BUFFER_LEN) != 0)
	    {
	        this->send<unsigned char>(&sync, 1);
	        this->receive<unsigned char>(chunk, file_size % SOCKET_MAX_BUFFER_LEN);
	        LOG_TI(Log4zBinary(&chunk,  file_size % SOCKET_MAX_BUFFER_LEN));//debug 2016-9-29
	        fp.write((char*)chunk, file_size % SOCKET_MAX_BUFFER_LEN);
	    }

	    fp.close();
	}

}//end namespace Socket
#endif
