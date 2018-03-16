// queue::push/pop
#include <iostream>       // std::cin, std::cout
#include <queue>          // std::queue
#include		<unistd.h>			// unix 系统服务函数定义
#include <pthread.h>
  std::queue<int> myqueue;

void *write_func(void *args){
	int myint;
	while(1)
	{
		std::cout << "Please enter some integers (enter 0 to end):\n";

		do {
		std::cin >> myint;
		myqueue.push (myint);
		} while (myint);
		break;
	}
}

void *read_func(void *args){
	int rec=1;
 	while(1)
	{
	  
	  while (!myqueue.empty())
	  {
	    std::cout << "myqueue contains: ";
	    rec = myqueue.front();
	    std::cout << ' ' << rec;
	    myqueue.pop();
	    std::cout << '\n';
	  }
	  if(rec==0)
	  {
		break;
	  }
	  
	  sleep(2);
	}

}
#ifndef main
int main ()
{

    pthread_t pid1;
    pthread_t pid2;
    //这种生产者和消费者都只有一个的情况下,这个循环缓冲区为竞争问题提供了免锁,大大提高了程序的处理效率
    pthread_create(&pid1,NULL,read_func,NULL);
    pthread_create(&pid2,NULL,write_func,NULL);

    pthread_join(pid1,NULL);
    pthread_join(pid2,NULL);

  return 0;
}
#endif