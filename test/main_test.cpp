#ifndef CPPCMS_TEST_H
#define CPPCMS_TEST_H

#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#define TEST(X)                                 \
    do {\
        if(X)\
        { std::cout<<"PASS!"<<std::endl;break; }\
        std::cout << "Error!" << __FILE__ << ":"<<__LINE__ << " "<<#X<<std::endl;  \
    }while(0)   
#ifdef USE_DI

#endif

int main(int argc,char *argv[])
{
	int result=0;
	#ifdef USE_DI
    std::cout<<"Test Version:"<<USE_DI<<std::endl;
    #endif
    if(argc>=2)
    {
	result=atoi(argv[1]);
	if(result==0)
	{
	    std::cout<<"Error Test param[1]:"<<result<<std::endl;
	    return -1;
	}
	}
	TEST(result>2);

	return 0;
}


#endif
