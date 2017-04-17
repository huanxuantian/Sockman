CC= gcc
CFLAGS= -O2 -Wall $(MYCFLAGS)
AR= ar rcu
RANLIB= ranlib
RM= rm -f
LIBS=

#MYCFLAGS=-ansi -std=c89 -pedantic
MYLDFLAGS= 
MYLIBS=

all: bar 

bar: bar.o
	$(CC)  -o $@ $^ 
clean:
	$(RM) bar *.o

# list targets that do not create files (but not all makes understand .PHONY)
.PHONY: all clean  
