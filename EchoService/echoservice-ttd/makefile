TARGET = client server
IDL = hello.thrift
SVRSRC = hello_server.cpp gen-cpp/HelloWorld.cpp
CLISRC = hello_client.cpp gen-cpp/HelloWorld.cpp

CC = g++
LIBS = -lthrift
CFLAGS = -Wall -std=c++14
THRIFT = thrift -gen cpp
# BOOST_FLAGS = -lboost_log
BOOST_FLAGS	= -DBOOST_LOG_DYN_LINK -I$(HOME)/include -I/usr/include -L$(HOME)/lib -lboost_log_setup -lboost_log -lboost_filesystem -lboost_system -lboost_thread -lboost_regex -lboost_date_time -lboost_chrono
PISTACHE_FLAGS = -lpistache
PTHREAD = -pthread
all: client.o server.o

gen-cpp/Message.cpp: $(IDL)
	$(THRIFT) $(IDL)

server.o: $(SVRSRC)
	$(CC) -o $@ $(SVRSRC) $(CFLAGS) $(LIBS)

client.o: $(CLISRC)
	$(CC) -o $@ $(CLISRC) $(LIBS) $(PISTACHE_FLAGS) $(PTHREAD) $(BOOST_FLAGS)
	
clean:
	$(RM) -r gen-cpp
	$(RM) $(TARGET)
