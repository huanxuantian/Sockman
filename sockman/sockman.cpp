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

using namespace zsummer::log4z;
using namespace std;

#define IP   "127.0.0.1"
//#define IP   "120.25.166.144"
#define PORT 10009

#define SEND_FILE "test.hex"

#define RECV_FILE "recv.hex"


int setup_loger()
{
	//start log4z
	cout<<__FUNCTION__<<"setup loger!!"<<endl;
   // ILog4zManager::getRef().setLoggerPath(LOG4Z_MAIN_LOGGER_ID, "./log2");
    ILog4zManager::getRef().start();
    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID,LOG_LEVEL_TRACE);
    //LOGD: LOG WITH level LOG_DEBUG
    //LOGI: LOG WITH level LOG_INFO
    cout<<__FUNCTION__<<("setup finish!!")<<endl;
    return 0;
}

//extern void LOG(int iLogType, char* szFmt, ...);

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
/*
void udp_receiver(void)
{
    try
    {
        Socket::UDP sock;
        double buffer[SOCKET_MAX_BUFFER_LEN];
        int i;

        sock.listen_on_port(10000);

        Socket::Datagram<string>            rec_str = sock.receive<string>();
        Socket::Datagram<int[5]>            rec_arr = sock.receive<int, 5>(); // ([, 5]);
        Socket::Datagram<float>             rec_var = sock.receive<float>();
        Socket::Datagram<double*>           rec_pnt = sock.receive<double>(buffer); // (buffer [, SOCKET_MAX_BUFFER_LEN]);
        Socket::Datagram<vector<prova> >    rec_vec = sock.receive<prova>(5); // conflict with the first one, must be specified

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
        Socket::Address to("127.0.0.1", 10000);

        sock.send<string>(to, "this is the string"); // ("127.0.0.1", 10000, "this is a string");
                                                     // as well as the others

        int iarr[5] = { 0, 1, 2, 3, 4 };
        sock.send<int>(to, iarr, 5);

        sock.send<float>(to, 5.0);

        double darr[5] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
        sock.send<double>(to, darr, 5);

        vector<prova> vec;
        for (i = 0; i < 5; i++) vec.push_back( { i, (float)(i + 1.0) });
        sock.send<prova>(to, vec);

        sock.close();
    }
    catch (Socket::SocketException &e)
    {
        cout << e << endl;
    }
}
*/

int main(int argc,char *argv[]){
    if(argc < 2)
    {
        printf("%s should not take %d agrement\n  USAGE: %s server \n\t %s client\n",argv[0],argc-1,argv[0],argv[0]);
        //argv[1] =(char*)"server";
        argv[1] =(char*)"client";
       printf("default to %s\n",argv[1]);

        //return -1;
    }
    setup_loger();
	LOGA("start test\r\n");
	if(strcmp(argv[1],"server")==0)
	{
		LOGI("start server for recive\r\n");
		tcp_receiver();
	}
	else if(strcmp(argv[1],"client")==0)
	{
		LOGI("start client for send "<<SEND_FILE);
		tcp_sender();
	}
	else
	{
		LOGI("unknow cmd"<<argv[1]);
	}
	puts("end\r\n");
	return EXIT_SUCCESS;
}
