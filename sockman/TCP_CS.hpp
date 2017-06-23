
#ifndef _TCP_CS_HPP_

#define _TCP_CS_HPP_

#include "CPPSocket/Socket.hpp"
//#include "./log4z/log4z.h"

using namespace std;
using namespace Socket;

#ifdef WINDOWS
#define HZ	1000

#else
	#include <pthread.h>
	#define USE_PTHREAD
	#define SOCKET SocketId
	#define LPVOID	void*
	#define HZ	1
#endif
namespace Socket
{

  class TCP_CS : public TCP
  {
  public://公开定义或类型
	#ifdef WINDOWS
	typedef void (WINAPI* TCP_CS_PROC)(TCP_CS* server,TCP_CS* client);
	#else
	typedef void (* TCP_CS_PROC)(TCP_CS* server,TCP_CS* client);
	#endif

  protected://保护的类型定义
  	typedef enum{
		TCP_CLIENT_TYPE=0,
		TCP_SERVER_TYPE=1,
		TCP_UNINIT_TYPE=2
	}TCP_CS_TYPE;
    struct TCP_CS_TIMEPARA//计算超时用的线程参数模板
	{
		int		OutTime;
		TCP_CS*	client;
		bool bExit;
		bool IsExit;
	};//线程函数必须staic，所以无this操作，必须传递对象
    struct TCP_CS_SERVERPARA//服务ACCECT客户的线程的参数模板
	{
		TCP_CS*	server;//服务监听socket对象
		TCP_CS_PROC	lpDealFunc;//透传到客户处理线程的回调函数
	};//线程函数必须staic，所以无this操作，必须传递对象

	struct TCP_CS_CLIENTPARA//客户处理线程参数模板
	{
		TCP_CS* client;//ACCECT后的客户socket对象
		TCP_CS* server;
		TCP_CS_PROC	lpDealFunc;//客户处理的回调函数
	};//线程函数必须staic，所以无this操作，必须传递对象

  public://公开的构造与操作相关函数
    //默认构造
	TCP_CS(void);
	//拷贝构造
	TCP_CS(const TCP_CS&);
	//析构
	~TCP_CS();
  protected://保护的内部变量
	//指定的服务对象（包含socket以及服务信息和接口），无用
    TCP _server;
	//指定的客户对象（包含socket以及服务信息和接口）,无用
	TCP _client;
	//是否在服务，只是标志，用于线程退出，为服务对象状态
	bool _bserver;
	//链接是否货活，只是标志，用于线程检测，为客户对象状态
	bool _bconnected;
	//类型，默认为TCP_UNINIT_TYPE，当启动服务后转成服务类型,当链接服务后转成客户类型
	//为初始化类型时，对应的操作都无法操作，返回未初始化的错误
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
  public://公开接口，业务接口
    //获取对象类型
    TCP_CS_TYPE get_tcp_type();
	//获取对象核心数据，如果是客户端对象，则返回监听对象信息，否则返回客户对象信息。
	TCP get_tcp();
	//获取指定对象链接状态
	//服务socket（监听socket）的状态除非退出服务，否则总返回true
	//客户sock（accect客户的socket）则根据select的客户状态返回
    bool is_connecteed(TCP_CS server); 
	//获取对象链接状态
	bool is_connecteed();
    //获取指定对象是否为服务状态，对于非监听socket永远返回false
    bool is_on_server(TCP_CS server); 
    //获取服务状态（在检测线程以及创建服务前检测用，返回_bserver，非监听socket永远返回false）
    bool is_on_server();
	//使用指定的地址结构创建服务（指定IP和端口）
	bool creat_server(Address bind_address,unsigned int listeners = 1);
	//使用指定端口创建服务，不指定IP的服务
	bool creat_server(Port port,unsigned int listeners = 1);
    //指定外部回调接口，外部处理
	bool start_server(TCP_CS_PROC lpDealFunc);
	//使用内部接口回调call_back
	bool start_server(void);
	//使用默认关闭流程函数
	bool stop_server(void);
	//
	bool stop_client(void);
	//接受客户端
	TCP_CS accept_client(void);
	//客户链接到服务地址
	void connect_to(Address address);
  protected://保护的内部接口
  	/*
	 *@超时控制线程
	 *@服务进行客户ACCECT线程
	 *@客户处理函数
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
	//@客户处理函数的默认回调函数
    static void call_back(TCP_CS* server,TCP_CS* client);
  };
}
#ifndef WINDOWS
#include "TCP_CS.cpp"
#endif
#endif
