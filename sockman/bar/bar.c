#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#define SHOW_ONLY
#define MAX_REPIX	60

void progress_bar_simple(int percent)
{
	/*show progress bar*/
    const int numTotal = MAX_REPIX;
    int numShow = (int)(numTotal * percent)/100;
    char count_x=percent%4;;

    if (numShow == 0)
        numShow = 1;

    if (numShow > numTotal)
        numShow = numTotal;
    if(percent==0)
    {
        count_x=0;
    }
    char sign[MAX_REPIX+1] = {0};
    if (percent == 100)
    {
       memset(sign, '=', numTotal);
       count_x=0;
    }
    else
    {
	    if(count_x==0)
	    {
		memset(sign, '=', numTotal);
		sign[numShow-2]='-';
		sign[numShow-1]='*';
		count_x++;
	    }
	    else if(count_x==1)
	    {
	      memset(sign, '=', numTotal);
		sign[numShow-2]='-';
		sign[numShow-1]='\\';
	      count_x++;
	    }
 	    else if(count_x==2)
	    {
	      memset(sign, '=', numTotal);
		sign[numShow-2]='-';
		sign[numShow-1]='-';
	      count_x++;
	    }
	    else if(count_x==3)
	    {
	      memset(sign, '=', numTotal);
		sign[numShow-2]='-';
		sign[numShow-1]='/';
	      count_x++;
	    }
    }
    count_x=count_x%4;
    printf("\r%-3d%%[%-*.*s] ", percent , numTotal, numShow, sign);
    fflush(stdout);

    if (numShow == numTotal)
    	printf("all finish!!!\r\n");
}

int s_port=0;
int main(int argc,char *argv[]){
    int i=0;   
    int j=0; 
    int test_count=0;
    char mode =0;
    if(argc < 2)
    {
        printf("%s should not take %d agrement\n  USAGE: %s [0-100] \n\t%s [0-100] [0-1]\n",argv[0],argc-1,argv[0],argv[0]);
        //argv[1] =(char*)"50";
        //argv[1] =(char*)"client";
       //printf("default to %s\n",argv[1]);

        return -1;
    }

    test_count=atoi(argv[1]);
    if(test_count>100)
    {
        printf("%s count %d error \n  USAGE:%s [0-100] \n\t%s [0-100] [0-1]\n",argv[0],test_count,argv[0],argv[0]);
        //argv[1] =(char*)"50";
        //argv[1] =(char*)"client";
       //printf("default to %s\n",argv[1]);

        return -1;	
    }
    if(argc>2)
    {
    mode = atoi(argv[2]);
    }
//#ifdef SHOW_ONLY
    if(!mode)
    {
    progress_bar_simple(test_count);
    }	
//#else
    else
    {
    while(test_count<101)
    {
        //for(i=0;i++;i<1000)
        if(i>10000)
        {
            j++;
            if(j%50000==0)
            {
                
                s_port++; //=(s_port+i)%101;

                
                s_port = s_port%101;
		progress_bar_simple(s_port);
                if(test_count==s_port)
                {
                    printf("\r\n");
                    s_port=0;
                    test_count++;
                    break;
                }
                
                /*
                if(s_port==0)
                {
                    test_count++;
                }
                */
                j=0;
                //i++;
            }
            i=0;
        }
        else
        {
            i++;
        }

    }
    }
 //#endif   
    return 0;
    //while(s_port<100);
}
