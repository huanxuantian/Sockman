CXXFLAGS = -std=c++0x -O2 -g -Wall -fmessage-length=0
OBJS =	sockman.o  jsoncpp.o log4z/log4z.o loger.o
#OBJS += TCPMan.o
#OBJS += CPPSocket/Address.o CPPSocket/CommonSocket.o CPPSocket/Datagram.o CPPSocket/SocketException.o CPPSocket/TCP.o 
#CPPSocket/UDP.o

LIBS =		

TARGET =	sockman

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) -lpthread

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *.hex ./log/*.log
