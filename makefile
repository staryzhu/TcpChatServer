

Support_Include= -I/. 
CXX	=g++
CXXFLAGS=  -fPIC -I. -I/usr/local/devsoft/include

RM	=rm -f 
AR	=ar

LDLIBS_DIR = -L/usr/local/devsoft/lib

LDLIBS_LIBS= -lpthread -ljson

.SUFFIXES:              .cpp
.cpp.o:
	$(RM) $@
	$(CXX) $(DEBUG) -c $(CXXFLAGS) -o $@ $<


OBJS = main.o TcpLoad.o EpollHandler.o


all:GameServer
TARGET = GameServer 


$(TARGET):$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(STATIC_LIBS) $(LIB_TOOLS) $(LDLIBS_DIR) $(LDLIBS_LIBS)


.PHOEY: debug clean
debug:
	make DEBUG='-D__DEBUG__ -g'
    
clean:
	rm -f *.o *.a $(TARGET)
    

