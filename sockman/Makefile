CXXFLAGS = -std=c++0x	-O2 -g -Wall -fmessage-length=0

OBJS =		sockman.o 

LIBS =

TARGET =	sockman

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
