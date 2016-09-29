//============================================================================
// Name        : sockman.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
using namespace std;

//#include "CPPSocket/Socket.hpp"
#include "TCPMan.hpp"
//#include "colorout/colorout.cpp"

#define IP   "127.0.0.1"
#define PORT 10000

extern void LOG(int iLogType, char* szFmt, ...);

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

        server.listen_on_port(10008,SOMAXCONN);
        Socket::TCP_MAN client = server.accept_client();

        cout << "receiving ..." << endl;
        client.receive_file("output.hex");
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
        server.send_file("input.hex");
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

int main(void) {

	//LOG(LOG_TYPE_NORMAL,"start test\r\n");
	tcp_receiver();
	puts("end\r\n");
	return EXIT_SUCCESS;
}
