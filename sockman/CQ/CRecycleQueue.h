//CRecycleQueue.h
#include<iostream>
using namespace std;
#define EXPMODE 0
#define INTMODE 1
//循环缓冲区类模板
template<class T>
class CRecycleQueue{

    private:
        //循环缓冲区地址指针
        T **_queue;
        //循环缓冲区读游标 （读的位置）
        int _read;
        //循环缓冲区写游标 （写的位置）
        int _write;
        //循环缓冲区的大小
        int _size;
        //我们姑且称这个变量为掩码,接下来用来作位&运算,从而实现循环缓冲
        int _mask;
       
    public:
        CRecycleQueue(){   
            _queue = NULL;
            _read = 0;
            _write = 0;
            _size = 0;
            _mask = 0;
        }
        //初始化循环缓冲区
        bool InitRecycleQueue(int exp,int mode=EXPMODE){
           
            if(0 > exp){

                return false;
            }

            _read = 0;
            _write = 0;
            //传进来一个整数，对1进行位移操作
            //比如exp = 4
            //_size的二进制表示:1000
            if(!mode)
            {
                _size = 1 << exp;
            }
            else
            {
                _size = exp;
            }
            //_mask的二进制表示:0111
            _mask = _size - 1;
            string m_str = mode?"intmode":"expmode";
            cout<<"mode"<<m_str<<",size:"<<_size<<",mask:"<<_mask<<endl;
            //分配缓冲区空间
            _queue = (T **)new char[sizeof (T *) * _size];

            if(NULL == _queue){

                return false;
            }

            return true;           
        }
/*
 *    size = 1000  mask = 0111
 *  write或read同mask 作&运算，可以实现循环缓冲区的功能
 *  也许你会问这里为什么不使用 % 运算实现循环的循环功能呢？
 *  答案是系统 & 运算效率要比 % 运算效率高
 *
 *    Push:
 *        write = 0;
 *            0000 & 0111 = 0; write++ (写入缓冲队列的第0位置)
 *        write = 1;
 *            0001 & 0111 = 1; write++ (写入缓冲队列的第1位置)
 *        write = 2;
 *            0010 & 0111 = 2; write++
 *        write = 3;
 *            0011 & 0111 = 3; write++
 *        ...
 *        write = 8;
 *            1000 & 0111 = 0; write++
 *        write = 9;
 *            1001 & 0111 = 1; write++
 *        ...
 *
 *    Pop:
 *        read = 0;
 *            0000 & 0111 = 0; read++ (读取缓冲队列的第0位置的数据)
 *        read = 1;
 *            0001 & 0111 = 1; read++ (读取缓冲队列的第1位置的数据)
 *        read = 2;
 *            0010 & 0111 = 2; read++
 *        read = 3
 *            0011 & 0111 = 3; read++
 *        ...
 *        read = 8;
 *            1000 & 0111 = 0; read++
 *        ...
 * */
       
        bool Push(T *type){
           
            if(NULL == type){

                return false;
            }
            //当条件不满足的时候,说明缓冲区已满,Push进来的数据就会丢失
            if(_write < _read + _size){
                //我们这里存入的是type指针，这个指针指向了一个我们分配的内存空间或者类
                _queue[_write & _mask] = type;
                _write++;
                return true;
            }
            cout<<"queue full!"<<endl;

            return false;
        }   

        T *Pop(){

            T *tmp = NULL;
            //当条件不满足的时候说明缓冲区已经没有数据
            if(_read < _write){
                //取出队列的数据
                tmp = _queue[_read & _mask];
                _read++;
            }

            return tmp;
        }
       
        int GetRemainSize(){

            return (_write - _read);
        }
};
