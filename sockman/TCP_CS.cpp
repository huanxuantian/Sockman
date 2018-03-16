#ifndef _TCP_CS_CPP_
#define _TCP_CS_CPP_

//#include "CPPSocket/Socket.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include "TCP_CS.hpp"

extern TCP_CS* new_client;
using namespace Socket;

using namespace std;

namespace Socket
{

	TCP_CS::TCP_CS(void)
	{
        this->_bserver = false;
		this->listen_port =0;
        this->_bconnected = false;
        this->_type=TCP_UNINIT_TYPE;
		CommonSocket(SOCK_STREAM);

	}

	TCP_CS::TCP_CS(const TCP_CS &tcp)
	{
		CommonSocket();
        this->_bserver = tcp._bserver;
        this->_bconnected = tcp._bconnected;
        this->_type = tcp._type;
		this->listen_port =tcp.listen_port;

		this->_socket_id = tcp._socket_id;
		this->_opened = tcp._opened;
		this->_binded = tcp._binded;
		this->_address = tcp._address;

	}
	TCP_CS::~TCP_CS()
	{
        this->_bserver = false;
        this->_bconnected = false;
		//~CommonSocket();
		//if(m_bNeedClose) Close();
		//if(m_bNeedCloseServer) CloseServer();
	}

     TCP_CS::TCP_CS_TYPE   TCP_CS::get_tcp_type()
    {
        return this->_type;
    }

    TCP TCP_CS::get_tcp()
    {
		TCP _tcp =TCP();
        return _tcp;
    }

    bool  TCP_CS::is_connecteed(TCP_CS server)
    {
        return server._bconnected;
    }

    bool TCP_CS::is_connecteed()
    {
     int nRet = 0;
     #ifdef WINDOWS
	struct fd_set Fd_Recv;
    #else
    fd_set Fd_Recv;
    #endif
    struct timeval Time_Recv;
    #ifdef WINDOWS
	memset(&Fd_Recv, 0, sizeof(struct fd_set));
    #else
    FD_ZERO(&Fd_Recv);
    #endif
    FD_CLR(this->_socket_id, &Fd_Recv); 
    FD_SET(this->_socket_id, &Fd_Recv); 
    Time_Recv.tv_sec = 0;
    Time_Recv.tv_usec = 0;

	nRet = select(this->_socket_id+1, &Fd_Recv, NULL, NULL, &Time_Recv);

    this->_bconnected = (nRet == 0);
        return this->_bconnected;
    }

    bool TCP_CS::is_on_server(TCP_CS server)
    {
        return server._bserver;
    }

    bool TCP_CS::is_on_server()
    {
        return this->_bserver;
    }

    bool TCP_CS::creat_server(Address bind_address,unsigned int listeners)
    {
       return creat_server(bind_address.port(),listeners);
    }

    bool TCP_CS::creat_server(Port port,unsigned int listeners)
    {
        CommonSocket::listen_on_port(port);

        if (listen(this->_socket_id, listeners) != 0)
        {
            stringstream error;
            error << "[creat_server] with [port=" << port << "] [listeners=" << listeners << "] Cannot listen socket";
            throw SocketException(error.str());
            return false;
        }
	this->listen_port = port;
        this->_type = TCP_SERVER_TYPE;
        this->_bserver = true;
        this->_bconnected = true;
        return true;
    }

    bool TCP_CS::start_server(TCP_CS_PROC lpDealFunc)
    {
        printf("%s,%d server socket %d ++++++\r\n",__FUNCTION__,__LINE__,this->_socket_id);
        TCP_CS_SERVERPARA* server_param;
        if(!this->_bserver||this->_type!=TCP_SERVER_TYPE)
        {
            return false;//not server 
        }
        printf("%s,%d server socket %d ++++++\r\n",__FUNCTION__,__LINE__,this->_socket_id);
        server_param = new  TCP_CS_SERVERPARA;
        server_param->server = this;
        server_param->lpDealFunc = lpDealFunc;

        #ifdef WINDOWS
        DWORD dwThreadId;
		m_hServerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TCP_CS::ServerProc, (LPVOID)(server_param), 0, &dwThreadId);
		if(m_hServerThread == NULL)
		{	
			return false;
		}
        #else
		int ret = pthread_create(&m_hServerThread,NULL,TCP_CS::ServerProc, (LPVOID)(server_param));
		if(ret!=0)
		{
			return false;
		}
		#endif
        printf("%s,%d server socket %d ++++++\r\n",__FUNCTION__,__LINE__,this->_socket_id);
        return true;
    }

    bool TCP_CS::start_server(void)
    {
         printf("%s,%d server socket %d ++++++\r\n",__FUNCTION__,__LINE__,this->_socket_id);
        return start_server((TCP_CS_PROC)TCP_CS::call_back);
    }

	bool TCP_CS::stop_server(void)
	{
		if(!this->_bserver||this->_type!=TCP_SERVER_TYPE)
		{
			return false;//not server 
		}
		printf("%s,%d stop server socket %d ++++++\r\n",__FUNCTION__,__LINE__,this->_socket_id);
		this->_bserver = false;
		this->close();
		//Sleep(3*HZ);
		return true;
	}

	bool TCP_CS::stop_client(void)
	{
		if(this->_bserver||this->_type!=TCP_CLIENT_TYPE||!this->_bconnected)
		{
			return false;//not client
		}
		printf("%s,%d stop client socket %d ++++++\r\n",__FUNCTION__,__LINE__,this->_socket_id);
		this->_bconnected = false;
		this->close();
		//Sleep(3*HZ);
		return true;
	}

    TCP_CS TCP_CS::accept_client(void)
    {
        TCP_CS ret;
        socklen_t len = sizeof(struct sockaddr_in);
        
        ret.close();
        ret._socket_id = accept(this->_socket_id, (struct sockaddr*)&ret._address, &len);
        ret._opened = true;
        ret._binded = true;
        ret._type = TCP_CLIENT_TYPE;
        ret._bserver = false;
        ret._bconnected = true;
        
        return ret;
    }

    void TCP_CS::connect_to(Address address)
    {
        TCP::connect_to(address);
        this->_type = TCP_CLIENT_TYPE;
        this->_bserver = false;
        this->_bconnected = true;
    }

    bool TCP_CS::GetLocalIP(int fd, std::string* local_ip, int* port) {
       struct sockaddr local_addr;
        socklen_t len = sizeof(sockaddr);
        if (getsockname(fd, &local_addr, &len) == 0) {
          struct sockaddr_in* sin = (struct sockaddr_in*)(&local_addr);
          *port = sin->sin_port;
          char addr_buffer[INET_ADDRSTRLEN];
          void * tmp = &(sin->sin_addr);
          if (inet_ntop(AF_INET, tmp, addr_buffer, INET_ADDRSTRLEN) == NULL){
            cerr << "inet_ntop err";
            return false;
          }   
          //cout << "addr:" << addr_buffer;
          if (local_ip != NULL) {
            local_ip->assign(addr_buffer);
          }   
          return true;
        } else {
          cerr << "getsockname err";
          return false;
        }
    }
    Address TCP_CS::get_address()
    {
        //this->_address
        string local_ip;
        int port;
        GetLocalIP(this->_socket_id,&local_ip,&port);

        //printf("connect address: %s:%d\r\n",local_ip.c_str(),port);

        this->_address.ip(local_ip);
        this->_address.port(port);

        return this->_address;
    }

    #ifdef WINDOWS
    void* CALLBACK TCP_CS::TimeoutControl(LPVOID lpParm)
    #else
	void*  TCP_CS::TimeoutControl(LPVOID lpParm)
    #endif
    {
        #ifdef WINDOWS
        return (void*)0;
        #else
        return NULL;
        #endif
    }
    #ifdef WINDOWS
	void* CALLBACK TCP_CS::ServerProc(LPVOID lpParm)
    #else
    void*  TCP_CS::ServerProc(LPVOID lpParm)
    #endif
    {
        TCP_CS_SERVERPARA* server_param;
        
        server_param = (TCP_CS_SERVERPARA*)lpParm;
        if(server_param==NULL)
        {
            #ifdef WINDOWS
            return (void*)0;
            #else
            return NULL;
            #endif
        }
		printf("%s,%d server object -> %ld++++++\r\n",__FUNCTION__,__LINE__,(long int)server_param->server);
        TCP_CS* pserver = server_param->server;
        TCP_CS_PROC lpDealFunc = server_param->lpDealFunc;
		printf("%s,%d server socket %d ++++++\r\n",__FUNCTION__,__LINE__,pserver->_socket_id);
        TCP_CS_CLIENTPARA* client_param;

        delete server_param;
        TCP_CS client;

 
       
        while(pserver->_bserver)
        {
           client = pserver->accept_client();

           client_param = new TCP_CS_CLIENTPARA;
           client_param->client = new TCP_CS(client);
           client_param->server = pserver;
           client_param->lpDealFunc = lpDealFunc;

           #ifdef WINDOWS
            DWORD dwThreadId;
            pserver->hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TCP_CS::DealProc, (LPVOID)(client_param), 0, &dwThreadId);
            if(pserver->hThread == NULL)
            {	
                delete client_param;
                #ifdef WINDOWS
                return (void*)0;
                #else
                return NULL;
                #endif
            }
            #else
            int ret = pthread_create(&pserver->hThread,NULL,DealProc, (LPVOID)(client_param));
            if(ret!=0)
            {
                delete client_param;
                #ifdef WINDOWS
                return -1;
                #else
                return NULL;
                #endif
            }
            #endif

        }
         printf("%s,%d server socket %d accept client %d ++++++\r\n",__FUNCTION__,__LINE__,pserver->_socket_id,client._socket_id);
        #ifdef WINDOWS
        return (void*)0;
        #else
        return NULL;
        #endif
    }
    #ifdef WINDOWS
	void* CALLBACK TCP_CS::DealProc(LPVOID lpParm)
    #else
    void*  TCP_CS::DealProc(LPVOID lpParm)
    #endif
    {
        TCP_CS_CLIENTPARA* client_param;
        client_param = (TCP_CS_CLIENTPARA*)lpParm;
         printf("%s,%d server socket %d <->client socket %d ++++++\r\n",__FUNCTION__,__LINE__,
			 client_param->server->_socket_id,client_param->client->_socket_id);
        if(client_param==NULL)
        {
            #ifdef WINDOWS
            return (void*)0;
            #else
            return NULL;
            #endif
        }

        TCP_CS* client = client_param->client;
        TCP_CS* server = client_param->server;
        TCP_CS_PROC lpDealFunc = client_param->lpDealFunc;
        delete client_param;

		//lpDealFunc(server,client);
		
        try
        {
            lpDealFunc(server,client);
        }
        catch(SocketException& me)
        {
	    cout<<me.what(); 
            #ifdef WINDOWS
            return (void*)0;
            #else
            return NULL;
            #endif
        }
	

        #ifdef WINDOWS
        //return 0;
		return (void*)0;
        #else
        return NULL;
        #endif
    }
//extern TCP_CS* new_client;
    void TCP_CS::call_back(TCP_CS* server,TCP_CS* client)
    {

         printf("%s,%d new socket %d accept ,start for server socket %d ++++++\r\n",__FUNCTION__,__LINE__,
			 client->_socket_id,server->_socket_id);
			
        printf("%s::client connect from %s:%d\n", __FUNCTION__,client->_address.ip().c_str(), client->_address.port());
			
	if(server->_socket_id<=0||client->_socket_id<=0)
	{
	    client->stop_client();
	    return;
	}
	if(new_client!=NULL)
	{
	    
	    if(new_client->is_connecteed())
	    {
		    printf("%s,%d,socket %d,need stop state:%d ++++++\r\n",__FUNCTION__,__LINE__,new_client->_socket_id,new_client->is_connecteed());
		    new_client->stop_client();
	    }
	}
	new_client = client;
        char rcv_buff[SOCKET_MAX_BUFFER_LEN]={0};
        int rcv_byte=0;
        int send_byte=0;
        int i=0;
        while(server->is_on_server()&&client->is_connecteed())
        {
            memset(rcv_buff,0,SOCKET_MAX_BUFFER_LEN);
            rcv_byte = client->receive<char>(rcv_buff,SOCKET_MAX_BUFFER_LEN);
            printf("%s,%d,socket %d,recv:%d,%d ++++++\r\n",__FUNCTION__,__LINE__,client->_socket_id,rcv_byte,client->is_connecteed());
            if(rcv_byte>0)
            {
                 printf("%s,%d socket %d echo:  ++++++\r\n",__FUNCTION__,__LINE__,client->_socket_id);
                 for(i=0;i<rcv_byte;i++)
                 {
					 printf("rcv_buff[%d]=0x%02x\r\n",i,rcv_buff[i]&0xff);
		}
		printf("%s,%d socket %d data end:  ++++++\r\n",__FUNCTION__,__LINE__,client->_socket_id);
		/*
                send_byte = client->send<char>(rcv_buff,rcv_byte);//echo only
                if(send_byte<0||send_byte!=rcv_byte)
                {
                    stringstream error;
                    error << "send echo data[" << send_byte << "]!=" << rcv_byte << "] in case for socket"<<client->_socket_id;
					 printf("%s,%d socket %d ++++++\r\n",__FUNCTION__,__LINE__,client->_socket_id);
                    throw SocketException(error.str());
                    
                }
		*/
            }
	    else if(rcv_byte ==0)
	    {
		if(client ==new_client)
		{
		new_client = NULL;
		}
		if(client->is_connecteed())
		{
			client->stop_client();
		}
		printf("%s,%d,exit server socket %d for port %d,state:%d,%d ++++++\r\n",__FUNCTION__,__LINE__,server->_socket_id,server->listen_port,server->is_on_server(),client->is_connecteed());
		return;
	    }
	    else
	    {
		 usleep(20*1000);
	    }
        }
		if(!client->is_connecteed())
		{
			if(client ==new_client)
			{
			new_client = NULL;
			}
			printf("%s,%d,exit handle for client socket %d,state:%d,%d ++++++\r\n",__FUNCTION__,__LINE__,client->_socket_id,server->is_on_server(),client->is_connecteed());
		}
		if(!server->is_on_server())
		{
			if(client ==new_client)
			{
			new_client = NULL;
			}
			if(client->is_connecteed())
			{
				client->stop_client();
			}
			printf("%s,%d,exit server socket %d for port %d,state:%d,%d ++++++\r\n",__FUNCTION__,__LINE__,server->_socket_id,server->listen_port,server->is_on_server(),client->is_connecteed());
		}

        
    }


}
#endif
