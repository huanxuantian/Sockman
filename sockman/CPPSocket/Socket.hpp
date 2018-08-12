
/*
 * Socket.hpp
 * This file is part of VallauriSoft
 *
 * Copyright (C) 2012 - Comina Francesco
 *
 * VallauriSoft is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * VallauriSoft is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with VallauriSoft; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef _SOCKET_HPP_

#define _SOCKET_HPP_

#include <iostream>
#include <sstream>
#include <exception>
#include <string>
#include <vector>
#include <fstream>

#if defined __WIN32 || defined __WIN64
    #define WINDOWS
#else
    #undef WINDOWS
#endif

#ifdef WINDOWS
    //#include <winsock.h>
	#include <WINSOCK2.h>
	#pragma comment(lib,"WS2_32.LIB")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
#endif

#ifdef WINDOWS

#else
	#include <pthread.h>
	#define USE_PTHREAD
#endif

#ifdef WINDOWS
#define HZ	1000

#else
	#include <pthread.h>
	#define USE_PTHREAD
	#define SOCKET SocketId
	#define LPVOID	void*
	#define HZ	1
#endif

#define SOCKET_MAX_BUFFER_LEN 1024

using namespace std;

namespace Socket
{
    typedef int SocketId;
    typedef string Ip;
    typedef unsigned int Port;
#ifdef WINDOWS
    typedef  int socklen_t;
#endif
    
    class SocketException : public exception
    {
    private:
        string _error;
    
    public:
        SocketException(const string&);
        ~SocketException() throw();
	
	    virtual const char* what() const throw();
        friend ostream& operator<< (ostream &out, SocketException &e);
    };

    struct Address : protected sockaddr_in
    {
    private:
        void _address(Ip, Port);

    public:
        Address();
        Address(Port);
        Address(Ip, Port);
        Address(struct sockaddr_in);
        Address(const Address&);

        Ip ip(void);
        Ip ip(Ip);

        Port port(void);
        Port port(Port);

        friend ostream& operator<< (ostream&, Address&);
    };

    template <class DataType>
    struct Datagram
    {
    public:
        Address address;
        DataType data;
        unsigned int received_bytes;
        unsigned int received_elements;

        template <class T>
        void operator= (const Datagram<T>&);
    };

    class CommonSocket
    {
    private:
#ifdef WINDOWS
        static unsigned int _num_sockets;
#endif
        void _socket(void);

    protected:
        SocketId _socket_id;
        int _socket_type;
        bool _opened;
        bool _binded;

    public:
        CommonSocket(void);
        CommonSocket(int);

        ~CommonSocket(void);

        void open(void);
        void close(void);
        
        virtual void listen_on_port(Port);
    };

    class UDP : public CommonSocket
    {
    public:
        UDP(void);
        UDP(const UDP&);
        //
        //template <class T> int send(Ip, Port, const T*, size_t);
		#ifdef WINDOWS
		template <class T>
		int send(Ip ip, Port port, const T *data, size_t len)
		{
			if (!this->_opened) this->open();

			len *= sizeof(T);
			if (len > (SOCKET_MAX_BUFFER_LEN * sizeof(T)))
			{
				stringstream error;
				error << "[send] with [ip=" << ip << "] [port=" << port << "] [data=" << data << "] [len=" << len << "] Data length higher then max buffer len";
				throw SocketException(error.str());
			}

			Address address(ip, port);
			int ret;

			if ((ret = sendto(this->_socket_id, (const char*)data, len, 0, (struct sockaddr*)&address, sizeof(struct sockaddr))) == -1)
			{
				stringstream error;
				error << "[send] with [ip=" << ip << "] [port=" << port << "] [data=" << data << "] [len=" << len << "] Cannot send";
				throw SocketException(error.str());
			}

			return ret;
		}
		//template <class T> int send(Address, const T*, size_t);
		template <class T>
		int send(Address address, const T *data, size_t len)
		{
			return this->send<T>(address.ip(), address.port(), data, len);
		}
		//template <class T> int send(Ip, Port, T);
		template <class T>
		int send(Ip ip, Port port, T data)
		{
			return this->send<T>(ip, port, &data, 1);
		}
		//template <class T> int send(Address, T);
		template <class T>
		int send(Address address, T data)
		{
			return this->send<T>(address.ip(), address.port(), &data, 1);
		}
		//template <class T> int send(Ip, Port, vector<T>);
		template <class T>
		int send(Ip ip, Port port, vector<T> data)
		{
			return this->send<T>(ip, port, data.data(), data.size());
		}
		//template <class T> int send(Address, vector<T>);
		template <class T>
		int send(Address address, vector<T> data)
		{
			return this->send<T>(address.ip(), address.port(), data.data(), data.size());
		}
		//template <class T> int receive(Address*, T*, size_t, unsigned int*);
		template <class T>
		int receive(Address *address, T *data, size_t len, unsigned int *received_elements)
		{
			if (!this->_opened) this->open();
			if (!this->_binded) throw SocketException("[receive] Make the socket listening before receiving");

			len *= sizeof(T);
			if (len > (SOCKET_MAX_BUFFER_LEN * sizeof(T)))
			{
				stringstream error;
				error << "[send] with [buffer=" << data << "] [len=" << len << "] Data length higher then max buffer length";
				throw SocketException(error.str());
			}

			int received_bytes;
			socklen_t size = sizeof(struct sockaddr);

			if ((received_bytes = recvfrom(this->_socket_id, (char*)data, len, 0, (struct sockaddr*)address, (socklen_t*)&size)) == -1)
			{
				throw SocketException("[receive] Cannot receive");
			}

			*received_elements = (unsigned int)(received_bytes / sizeof(T));

			return received_bytes;
		}
		//template <class T> Datagram<T*> receive(T*, size_t len = SOCKET_MAX_BUFFER_LEN);
		template <class T>
		Datagram<T*> receive(T *buffer, size_t len = SOCKET_MAX_BUFFER_LEN)
		{
			Datagram<T*> ret;

			ret.received_bytes = this->receive<T>(&ret.address, buffer, len, &ret.received_elements);
			ret.data = buffer;

			return ret;
		}
		//template <class T, size_t N> Datagram<T[N]> receive(size_t len = N);
		template <class T, size_t N>
		Datagram<T[N]> receive(size_t len= N)
		{
			Datagram<T[N]> ret;
			ret.received_bytes = this->receive<T>(&ret.address, ret.data, len, &ret.received_elements);
			return ret;
		}
		//template <class T> Datagram<T> receive(void);
		template <class T>
		Datagram<T> receive(void)
		{
			Datagram<T> ret;
			ret.received_bytes = this->receive<T>(&ret.address, &ret.data, 1, &ret.received_elements);
			return ret;
		}
		//template <class T> Datagram<vector<T> > receive(size_t);
		template <class T>
		Datagram<vector<T> > receive(size_t len)
		{
			Datagram<vector<T> > ret;
			T* buffer =new T[len];

			ret.received_bytes = this->receive<T>(&ret.address, buffer, len, &ret.received_elements);
			for (int i = 0; i < (int)ret.received_elements; i++) ret.data.push_back(buffer[i]);

			return ret;
		}
		template <>
		Datagram<string> receive<string>(void)
		{
			Datagram<string> ret;
			char buffer[SOCKET_MAX_BUFFER_LEN];

			ret.received_bytes = this->receive<char>(&ret.address, buffer, SOCKET_MAX_BUFFER_LEN, &ret.received_elements);
			ret.data = buffer;

			return ret;
		}

		template <>
		int send<string>(Ip ip, Port port, string data)
		{
			return this->send<char>(ip, port, data.c_str(), data.length() + 1);
		}

		template <>
		int send<string>(Address address, string data)
		{
			return this->send<char>(address.ip(), address.port(), data.c_str(), data.length() + 1);
		}
		#else
		template <class T> int send(Ip, Port, const T*, size_t);
		template <class T> int send(Address, const T*, size_t);
		template <class T> int send(Ip, Port, T);
		template <class T> int send(Address, T);
		template <class T> int send(Ip, Port, vector<T>);
		template <class T> int send(Address, vector<T>);
		
		template <class T> int receive(Address*, T*, size_t, unsigned int*);
		template <class T> Datagram<T*> receive(T*, size_t len= SOCKET_MAX_BUFFER_LEN);
		template <class T, size_t N> Datagram<T[N]> receive(size_t len=N);
		template <class T> Datagram<T> receive(void);
		template <class T> Datagram<vector<T> > receive(size_t);
		#endif

    };

    class TCP : public CommonSocket
    {
    private:
        Address _address;
    public:
        TCP(void);
        TCP(const TCP&);
        
        Ip ip(void);
        Port port(void);
        Address address(void);
        
        void listen_on_port(Port, unsigned int listeners = 1);
        void connect_to(Address);
        
        TCP accept_client(void);
		//template <class T> int send(const T*, size_t);
	#ifdef WINDOWS
		template <class T>
		int send(const T* buffer, size_t len)
		{
			if (!this->_binded) throw SocketException("[send] Socket not binded");
			if (!this->_opened) throw SocketException("[send] Socket not opened");

			len *= sizeof(T);
			if (len > (SOCKET_MAX_BUFFER_LEN * sizeof(T)))
			{
				stringstream error;
				error << "[send] [len=" << len << "] Data length higher then max buffer len (" << SOCKET_MAX_BUFFER_LEN << ")";
				throw SocketException(error.str());
			}

			int ret;
			if ((ret = ::send(this->_socket_id, (const char*)buffer, len, 0)) == -1) throw SocketException("[send] Cannot send");
			return ret;
		}
		//template <class T> int receive(T*, size_t);
		template <class T>
		int receive(T* buffer, size_t len)
		{
			if (!this->_binded) throw SocketException("[send_file] Socket not binded");
			if (!this->_opened) throw SocketException("[send_file] Socket not opened");

			len *= sizeof(T);
			if (len > (SOCKET_MAX_BUFFER_LEN * sizeof(T)))
			{
				stringstream error;
				error << "[receive] [len=" << len << "] Data length higher then max buffer len (" << SOCKET_MAX_BUFFER_LEN << ")";
				throw SocketException(error.str());
			}

			int ret;
			if ((ret = recv(this->_socket_id, (char*)buffer, len, 0)) == -1) throw SocketException("[send] Cannot receive");
			return ret;
		}

	#else  
	template <class T> int send(const T*, size_t);   
	template <class T> int receive(T*, size_t);   
	#endif
        
        void send_file(string);
        void receive_file(string);
    };
    	#ifdef WINDOWS
	//typedef void (CALLBACK* DEALPROC)(SocketId s, int ServerPort, const char *ClientIP);
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
  class TCP_CS : public TCP
  {
  public://�������������
	#ifdef WINDOWS
	typedef void (WINAPI* TCP_CS_PROC)(TCP_CS* server,TCP_CS* client);
	#else
	typedef void (* TCP_CS_PROC)(TCP_CS* server,TCP_CS* client);
	#endif

  protected://���������Ͷ���
  	typedef enum{
		TCP_CLIENT_TYPE=0,
		TCP_SERVER_TYPE=1,
		TCP_UNINIT_TYPE=2
	}TCP_CS_TYPE;
    struct TCP_CS_TIMEPARA//���㳬ʱ�õ��̲߳���ģ��
	{
		int		OutTime;
		TCP_CS*	client;
		bool bExit;
		bool IsExit;
	};//�̺߳�������staic��������this���������봫�ݶ���
    struct TCP_CS_SERVERPARA//����ACCECT�ͻ����̵߳Ĳ���ģ��
	{
		TCP_CS*	server;//�������socket����
		TCP_CS_PROC	lpDealFunc;//͸�����ͻ������̵߳Ļص�����
	};//�̺߳�������staic��������this���������봫�ݶ���

	struct TCP_CS_CLIENTPARA//�ͻ������̲߳���ģ��
	{
		TCP_CS* client;//ACCECT��Ŀͻ�socket����
		TCP_CS* server;
		TCP_CS_PROC	lpDealFunc;//�ͻ�����Ļص�����
	};//�̺߳�������staic��������this���������봫�ݶ���

  public://�����Ĺ����������غ���
    //Ĭ�Ϲ���
	TCP_CS(void);
	//��������
	TCP_CS(const TCP_CS&);
	//����
	~TCP_CS();
  protected://�������ڲ�����
	//ָ���ķ�����󣨰���socket�Լ�������Ϣ�ͽӿڣ�������
    TCP _server;
	//ָ���Ŀͻ����󣨰���socket�Լ�������Ϣ�ͽӿڣ�,����
	TCP _client;
	//�Ƿ��ڷ���ֻ�Ǳ�־�������߳��˳���Ϊ�������״̬
	bool _bserver;
	//�����Ƿ���ֻ�Ǳ�־�������̼߳�⣬Ϊ�ͻ�����״̬
	bool _bconnected;
	//���ͣ�Ĭ��ΪTCP_UNINIT_TYPE�������������ת�ɷ�������,�����ӷ����ת�ɿͻ�����
	//Ϊ��ʼ������ʱ����Ӧ�Ĳ������޷�����������δ��ʼ���Ĵ���
	TCP_CS_TYPE _type;
 private:
 	#ifdef WINDOWS
		HANDLE	m_hServerThread;
	#else
		pthread_t m_hServerThread;
	#endif
#ifdef WINDOWS
		HANDLE	hThread;
#else
		pthread_t hThread;
#endif
    Address _address;
	Port listen_port;
	bool GetLocalIP(SocketId fd, std::string* local_ip, int* port);
  public://�����ӿڣ�ҵ��ӿ�
    //��ȡ��������
    TCP_CS_TYPE get_tcp_type();
	//��ȡ����������ݣ�����ǿͻ��˶����򷵻ؼ���������Ϣ�����򷵻ؿͻ�������Ϣ��
	TCP get_tcp();
	//��ȡָ����������״̬
	//����socket������socket����״̬�����˳����񣬷����ܷ���true
	//�ͻ�sock��accect�ͻ���socket�������select�Ŀͻ�״̬����
    bool is_connecteed(TCP_CS server); 
	//��ȡ��������״̬
	bool is_connecteed();
    //��ȡָ�������Ƿ�Ϊ����״̬�����ڷǼ���socket��Զ����false
    bool is_on_server(TCP_CS server); 
    //��ȡ����״̬���ڼ���߳��Լ���������ǰ����ã�����_bserver���Ǽ���socket��Զ����false��
    bool is_on_server();
	//ʹ��ָ���ĵ�ַ�ṹ��������ָ��IP�Ͷ˿ڣ�
	bool creat_server(Address bind_address,unsigned int listeners = 1);
	//ʹ��ָ���˿ڴ������񣬲�ָ��IP�ķ���
	bool creat_server(Port port,unsigned int listeners = 1);
    //ָ���ⲿ�ص��ӿڣ��ⲿ����
	bool start_server(TCP_CS_PROC lpDealFunc);
	//ʹ���ڲ��ӿڻص�call_back
	bool start_server(void);
	//ʹ��Ĭ�Ϲر����̺���
	bool stop_server(void);
	//
	bool stop_client(void);
	//���ܿͻ���
	TCP_CS accept_client(void);
	//�ͻ����ӵ������ַ
	void connect_to(Address address);
	
	Address get_address();
  protected://�������ڲ��ӿ�
  	/*
	 *@��ʱ�����߳�
	 *@������пͻ�ACCECT�߳�
	 *@�ͻ�������
	*/
    #ifdef WINDOWS
	static void* CALLBACK TimeoutControl(LPVOID lpParm);
	static void* CALLBACK ServerProc(LPVOID lpParm);
	static void* CALLBACK DealProc(LPVOID lpParm);
	#else
	static void*  TimeoutControl(LPVOID lpParm);
	static void*  ServerProc(LPVOID lpParm);
	static void*  DealProc(LPVOID lpParm);
	#endif
	//@�ͻ���������Ĭ�ϻص�����
    static void call_back(TCP_CS* server,TCP_CS* client);
  };
}

#ifndef WINDOWS
#include "SocketException.cpp"
#include "CommonSocket.cpp"
#include "Datagram.cpp"
#include "Address.cpp"
#include "UDP.cpp"
#include "TCP.cpp"
#include "TCPMan.cpp"
#include "TCP_CS.cpp"
#endif


#endif

