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

#include "TCPMan.hpp"
#include "./log4z/log4z.h"
#include "./json/json.h"

using namespace zsummer::log4z;
using namespace Json;
using namespace std;

#define IP   "127.0.0.1"
//#define IP   "120.25.166.144"
#define PORT 10009

#define SEND_FILE "test.hex"

#define RECV_FILE "recv.hex"
/*
LoggerId logid_test;
int setup_loger()
{
	//start log4z
	cout<<__FUNCTION__<<"setup loger!!"<<endl;
	logid_test = ILog4zManager::getRef().createLogger("test" );
    ILog4zManager::getRef().config("config.cfg");

	ILog4zManager::getRef().setLoggerDisplay(logid_test, true);
	ILog4zManager::getRef().setLoggerOutFile(logid_test, true);
    ILog4zManager::getRef().start();
   // ILog4zManager::getRef().setLoggerPath(LOG4Z_MAIN_LOGGER_ID, "./log2");
    //ILog4zManager::getRef().start();
    //ILog4zManager::getRef().setLoggerLevel(LOG4Z_TESTLOGGER_ID,LOG_LEVEL_TRACE);
    //ILog4zManager::getRef().setLoggerName(LOG4Z_TESTLOGGER_ID,"test");
    //LOGD: LOG WITH level LOG_DEBUG
    //LOGI: LOG WITH level LOG_INFO
    cout<<__FUNCTION__<<("setup finish!!")<<endl;
    return 0;
}
*/
//extern void LOG(int iLogType, char* szFmt, ...);
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


void tcp_receiver(void)
{
    try
    {
        Socket::TCP_MAN server;

        server.listen_on_port(PORT,SOMAXCONN);
        Socket::TCP_MAN client = server.accept_client();

        cout << "receiving ..." << endl;
        client.receive_file(RECV_FILE);
    }
    catch (Socket::SocketException &e)
    {
        cout << e << endl;
    }
}

void tcp_sender(void)
{
    try
    {
        Socket::TCP_MAN server;
        server.connect_to(Socket::Address(IP, PORT));

        cout << "sending ..." << endl;
        server.send_file(SEND_FILE);
    }
    catch (Socket::SocketException &e)
    {
        cout << e << endl;
    }
}

void udp_receiver(void)
{
    try
    {
        Socket::UDP sock;
        double buffer[SOCKET_MAX_BUFFER_LEN];
        int i;

        sock.listen_on_port(PORT);

        Socket::Datagram<string>            rec_str = sock.receive<string>();
        LOG_TA("string data:"<<Log4zBinary(rec_str.data.c_str(),strlen(rec_str.data.c_str())));//log
        Socket::Datagram<int[5]>            rec_arr = sock.receive<int, 5>(); // ([, 5]);
        LOG_TA("int array:"<<Log4zBinary(rec_arr.data,sizeof(rec_arr.data)));//log
        Socket::Datagram<float>             rec_var = sock.receive<float>();
        LOG_TA("float:"<<Log4zBinary((char*)&rec_var.data,sizeof(rec_var.data)));//log
        Socket::Datagram<double*>           rec_pnt = sock.receive<double>(buffer); // (buffer [, SOCKET_MAX_BUFFER_LEN]);
        LOG_TA("double array"<<Log4zBinary((char*)rec_pnt.data,sizeof(rec_pnt.data)*rec_pnt.received_elements));//log
        Socket::Datagram<vector<prova> >    rec_vec = sock.receive<prova>(5); // conflict with the first one, must be specified
        LOG_TA("vec data:"<<Log4zBinary((char*)&rec_vec.data,sizeof(rec_vec.data)*rec_vec.received_elements));//log
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
    	int i;
        Socket::UDP sock;
        Socket::Address to(IP, PORT);

        sock.send<string>(to, "this is the string"); // ("127.0.0.1", 10000, "this is a string");
                                                     // as well as the others

        int iarr[5] = { 0, 1, 2, 3, 4 };
        LOG_TA("int array:"<<Log4zBinary((char*)&iarr,sizeof(iarr)));//log
        sock.send<int>(to, iarr, 5);

        sock.send<float>(to, 5.0);

        double darr[5] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
        LOG_TA("double array:"<<Log4zBinary((char*)&darr,sizeof(darr)));//log
        sock.send<double>(to, darr, 5);

        vector<prova> vec;
        for (i = 0; i < 5; i++) vec.push_back( { i, (float)(i + 1.0) });
        LOG_TA("vector data::"<<Log4zBinary((char*)vec.data(),sizeof(vec.data())*vec.size()));//log
        sock.send<prova>(to, vec);

        sock.close();
    }
    catch (Socket::SocketException &e)
    {
        cout << e << endl;
    }
}


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
	LOG_TA("start test\r\n");
	if(strcmp(argv[1],"server")==0)
	{
		LOG_TI("start server for recive\r\n");
		tcp_receiver();
	}
	else if(strcmp(argv[1],"client")==0)
	{
		LOG_TI("start client for send "<<SEND_FILE);
		json_test1();
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
	else
	{
		LOG_TI("unknow cmd"<<argv[1]);
	}
	puts("end\r\n");
	return EXIT_SUCCESS;
}
