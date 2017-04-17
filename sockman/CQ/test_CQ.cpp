//main.cpp

#include <iostream>
#include		<unistd.h>			// unix 系统服务函数定义
#include <pthread.h>
#include "CRecycleQueue.h"
using namespace std;

class UserInfo{

    private :
        int _num;
        time_t cur_time;
    public:
        UserInfo(int num){

            _num = num;
            time(&cur_time);
        }

        int getUserNum(){

            return _num;
        }
        int getUserTime(){
            return (int)cur_time;
        }
};

CRecycleQueue<UserInfo> *cq_queue = new CRecycleQueue<UserInfo>;

void *write_func(void *args){

    int num = 0;
    while(1){

        //UserInfo里可以封装你自己想要的数据
        //这里仅仅是一个简单的测试用例   
        UserInfo *info = new UserInfo(num++);

        if(!cq_queue->Push(info)){
            //Push失败 删除手动分配的内存空间   
            delete info;
        }
        sleep(2);
    }
}

void *read_func(void *args){

    while(1){

        UserInfo *info = NULL;
        if(info = cq_queue->Pop()){

            cout<<"pop:num:"<<info->getUserNum()<<",time:"<<info->getUserTime()<<endl;
            delete info;
        }
        sleep(1);
    }
}
#ifndef main
int main(){

	cq_queue->InitRecycleQueue(8,INTMODE);

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
