//============================================================================
// Name        : sockman.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description :a socket manager system and message center model
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#include "loger.hpp"
#include "CPPSocket/Socket.hpp"
#include "./log4z/log4z.h"
#include "./json/json.h"
//#include "./CQ/CRecycleQueue.h"


TCP_CS* new_client;
TCP_CS server;

TCP_CS c_client;

using namespace zsummer::log4z;
using namespace Json;
using namespace std;

unsigned char test_data[] ={0x7E ,0x07 ,0x86 ,0x02 ,0x00 ,0x3F ,0x00 ,0x00 ,0x00 ,0x5E ,0x01 ,0xE3 ,0x7E};

#define IP   "127.0.0.1"
//#define IP   "120.25.166.144"
#define PORT 10009

int s_port=PORT;

char* s_ip=(char*)IP;

#define SEND_FILE "test.hex"

#define RECV_FILE "recv.hex"

Value root;
Reader reader;
StyledWriter writer;

int json_test1()
{
	LOGA( __FUNCTION__<<":read string and parse json rewrite json");
	LOG_TI("input:"<< "json{\"Device_COM\":\"-1\",\"state\":-1,\"result\":true}");
	LOG_TI("rewrite:"<< "Device_COM=\"2\",state=0,result=false");
	bool parsingSuccessful = reader.parse("{\"Device_COM\":\"-1\",\"state\":-1,\"result\":true}", root );
	if(!parsingSuccessful)
	{
		LOG_TE("\r\nfaile\r\n");
		return -1;
	}
	string m_COM =root.get("Device_COM","0").asCString();
	int m_state =root.get("state","0").asInt();
	bool m_result =root.get("result",false).asBool();

	LOG_TI("Reader1:Device_COM="<<m_COM<<",state="<<m_state<<",result="<<m_result);

	root["Device_COM"] ="3";
	root["state"] =0;
	root["result"] =false;
	string m_state2 =root.get("Device_COM","0").asCString();

	string out = writer.write( root );
	LOG_TI("Writer1:"<<(out));

	LOG_TA(Log4zBinary(out.c_str(),strlen(out.c_str())));

	 fstream fp(SEND_FILE, ios::out | ios::binary);
	 if (!fp.is_open())
	{
		return -2;
	}
	fp.seekg(0, ifstream::beg);
	const char* chunk = out.c_str();
	int len = strlen(out.c_str());
	fp.write((char*)chunk, len);
	fp.close();

	LOG_TA( __FUNCTION__<<":finish!");
	return 0;
}

struct prova
{
    int something;
    float somethingelse;
};

//CRecycleQueue<Socket::TCP_MAN> *tcp_queue = new CRecycleQueue<Socket::TCP_MAN>;
/*@brief test for tcp server mode */
void tcp_receiver(void)
{
    try
    {
        Socket::TCP_MAN server;
        //tcp_queue->InitRecycleQueue(3);
        server.listen_on_port(s_port,SOMAXCONN);
        Socket::TCP_MAN client = server.accept_client();
        //tcp_queue->Push(&client);
        cout << "receiving ..." << endl;
        Socket::TCP_MAN* h_client=&client;//tcp_queue->Pop();
        h_client->receive_file(RECV_FILE);
    }
    catch (Socket::SocketException &e)
    {
        cout << e << endl;
    }
}
/*@brief test for tcp server mode */
void tcp_sender(void)
{
    try
    {
        Socket::TCP_MAN server;
        server.connect_to(Socket::Address(s_ip, s_port));

        cout << "sending ..." << endl;
        server.send_file(SEND_FILE);
    }
    catch (Socket::SocketException &e)
    {
        cout << e << endl;
    }
}
#if 1
void udp_receiver(void)
{
    try
    {
        Socket::UDP sock;

        sock.listen_on_port(PORT);

        Socket::Datagram<string>            rec_str = sock.receive<string>();
        LOG_TA("string data:"<<Log4zBinary(rec_str.data.c_str(),strlen(rec_str.data.c_str())));//log
        Socket::Datagram<int[5]>            rec_arr = sock.receive<int, 5>(); // ([, 5]);
        LOG_TA("int array:"<<Log4zBinary(rec_arr.data,sizeof(rec_arr.data)));//log
        Socket::Datagram<float>             rec_var = sock.receive<float>();
        LOG_TA("float:"<<Log4zBinary((char*)&rec_var.data,sizeof(rec_var.received_bytes)));//log
		
		/*
		double buffer[SOCKET_MAX_BUFFER_LEN];
		int i;
		 
		Socket::Datagram<double*>           rec_pnt = sock.receive<double>(buffer); // (buffer [, SOCKET_MAX_BUFFER_LEN]);
        LOG_TA("double array"<<Log4zBinary((char*)rec_pnt.data,sizeof(rec_pnt.data)*rec_pnt.received_elements));//log

		Socket::Datagram<vector<prova> >    rec_vec = sock.receive<prova>(5); // conflict with the first one, must be specified
        for (i = 0; i < (int)rec_vec.data.size(); i++)
        LOG_TA("vec data["<<i<<"]:"<<Log4zBinary((char*)&rec_vec.data[i],sizeof(prova)));//log
		*/
        /*
        cout << rec_str.data << endl;
        cout << endl;
        for (i = 0; i < 5; i++) cout << rec_arr.data[i] << endl;
        cout << endl;
        cout << rec_var.data << endl;
        cout << endl;
        for (i = 0; i < (int)rec_pnt.received_elements; i++) cout << rec_pnt.data[i] << endl;
        cout << endl;
        for (i = 0; i < (int)rec_vec.data.size(); i++) cout << rec_vec.data[i].something << " - " << rec_vec.data[i].somethingelse << endl;
        cout << endl;
        cout << "from: " << rec_vec.address << endl; // I know could be always different, just for the sake of simplicity
	*/
        sock.close();
    }
    catch (Socket::SocketException &e)
    {
        cout << e << endl;
    }
}

void udp_sender(void)
{
    try
    {
    	
        Socket::UDP sock;
        Socket::Address to(IP, PORT);

        sock.send<string>(to, "this is the string"); // ("127.0.0.1", 10000, "this is a string");
                                                     // as well as the others

        int iarr[5] = { 0, 1, 2, 3, 4 };
		float f_send=5.0;
        LOG_TA("int array:"<<Log4zBinary((char*)&iarr,sizeof(iarr)));//log

		 //sock.send<INT8>(to, 'A');
		
        sock.send<int>(to, iarr, 5);
		LOG_TA("float:"<<Log4zBinary((char*)&f_send,sizeof(float)));//log
        sock.send<float>(to, 5.0);
		
		/*
        double darr[5] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
        LOG_TA("double array:"<<Log4zBinary((char*)&darr,sizeof(darr)));//log
        sock.send<double>(to, darr, 5);
		
        vector<prova> vec;
        int i;
        for (i = 0; i < 5; i++) vec.push_back( { i, (float)(i + 1.0) });
        LOG_TA("vector data::"<<Log4zBinary((char*)vec.data(),sizeof(vec.data())*vec.size()));//log
        sock.send<prova>(to, vec);
		*/
        sock.close();
    }
    catch (Socket::SocketException &e)
    {
        cout << e << endl;
    }
}
#endif
bool server_ok =false;

int main(int argc,char *argv[]){
    if(argc < 2)
    {
        printf("%s should not take %d agrement\n  USAGE: %s server \n\t %s client\n\t%s udpsrv\n\t %s udpcli\n",argv[0],argc-1,argv[0],argv[0],argv[0],argv[0]);
        //argv[1] =(char*)"server";
        //argv[1] =(char*)"client";
       //printf("default to %s\n",argv[1]);

        return -1;
    }
    setup_loger();
	int count =0;
	LOG_TA("start test\r\n");
	if(strcmp(argv[1],"server")==0)
	{
		LOG_TI("start server for recive\r\n");
        if(argc>=3)
        {
            s_port=atoi(argv[2]);
            LOG_TA("server listen_on port "<<s_port);
        }
		tcp_receiver();
	}
	else if(strcmp(argv[1],"client")==0)
	{
		LOG_TI("start client for send "<<SEND_FILE);
		json_test1();
        if(argc>=3)
        {
            s_port=atoi(argv[2]);
            if(argc>=4)
            {
                s_ip=argv[3];
                LOG_TA("start client connect to ip "<<s_ip);
            }
            LOG_TA("client connect to port "<<s_port);
        }
        
		tcp_sender();
	}
	else if(strcmp(argv[1],"udpcli")==0)
	{
		LOG_TI("start client for udp test "<<SEND_FILE);
		udp_sender();
	}
	else if(strcmp(argv[1],"udpsrv")==0)
	{
		LOG_TI("start serve for udp test "<<SEND_FILE);
		udp_receiver();
	}
	else if(strcmp(argv[1],"cserver")==0)
	{
		if(argc>=3)
		{
			s_port=atoi(argv[2]);
			LOG_TA("server listen_on port "<<s_port);
			while(count<30)
			{
			    try
			    {

				    server_ok = server.creat_server(s_port,10);
				    if(server_ok)
				    {
				    server.start_server();
				    break;
				    }

			    }
			    catch(SocketException& me)
			    {
				LOG_TE(me.what());
				server.stop_server();
				    //puts("end\r\n");
				    //return -1;
			    }
			    sleep(10);
			    count++;
			}
			if(count==30)
			{
				server.stop_server();
				puts("end\r\n");
				return -1; 
			}
		}

		while(1)
		    {

			 try
		        {
		        if(new_client!=NULL&&new_client->is_connecteed())
			    {
				int i;
				int data_len =sizeof(test_data);
				int n_send_byte = new_client->send<char>((char*)test_data,data_len);

				LOG_TA(__FUNCTION__<<"::"<<__LINE__<<" socket sned start len:"<<n_send_byte);
				     for(i=0;i<data_len;i++)
				     {
					    printf("buff[%d]:0x%02X\r\n",i,test_data[i]&0xff);
					    if(i>=16)
					      {
						 printf("msg too long not display other bytes\r\n");
					         break;
					      }
				    }
				    LOG_TA(__FUNCTION__<<"::"<<__LINE__<<" socket data end:  ++++++\r\n");
				//printf("send data to %d!!!\r\n",new_client->_socket_id);
				sleep(20);
				break;
			    }
			    else
			    {
				sleep(1);
			    }
		        }
		        catch(SocketException& me)
		        {
		            LOG_TE(me.what());
		        }

			//test_data
		    }

	}
	else if(strcmp(argv[1],"cclient")==0)
	{
		LOG_TI("start c client for send "<<SEND_FILE);
		//json_test1();
        if(argc>=3)
        {
            s_port=atoi(argv[2]);
            if(argc>=4)
            {
                s_ip=argv[3];
                LOG_TA("start c client connect to ip "<<s_ip);
            }
            LOG_TA("client c connect to port "<<s_port);
		}
		Socket::Address to(s_ip, s_port);
		c_client.connect_to(to);
		{
			LOG_TA(__FUNCTION__<<"::client connect from "<<c_client.get_address().ip().c_str()<<":"<<c_client.get_address().port());
		}
		while(c_client.is_connecteed())
		{
			sleep(10);
			//printf("%s::client connect from %s:%d\n", __FUNCTION__,c_client.get_address().ip().c_str(), c_client.get_address().port());
		}
		
	}
	else
	{
		LOG_TI("unknow cmd"<<argv[1]);
	}
	server.stop_server();
	puts("end\r\n");
	return EXIT_SUCCESS;
}
