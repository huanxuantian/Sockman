CC= g++
CFLAGS= -O2 -Wall $(MYCFLAGS) 
AR= ar rcu
RANLIB= ranlib
RM= rm -f
LIBS=pthread

#MYCFLAGS=-ansi -std=c89 -pedantic
MYLDFLAGS= 
MYLIBS=

all: queue_test 

queue_test: queue_test.o
	$(CC)  -o $@  $^ -lpthread
clean:
	$(RM) queue_test *.o

# list targets that do not create files (but not all makes understand .PHONY)
.PHONY: all clean  
