TARGET = client server
IDL = uniqueid.thrift
SVRSRC = echo_server.cpp gen-cpp/EchoService.cpp gen-cpp/echoservice_types.cpp
CLISRC = echo_client.cpp gen-cpp/EchoService.cpp gen-cpp/echoservice_types.cpp

CC = g++
LIBS = -lthrift -ltracer
CFLAGS = -Wall -std=c++17
THRIFT = thrift -gen cpp
# BOOST_FLAGS = -lboost_log
BOOST_FLAGS	= -DBOOST_LOG_DYN_LINK -I$(HOME)/include -I/usr/include -L$(HOME)/lib -lboost_log_setup -lboost_log -lboost_filesystem -lboost_system -lboost_thread -lboost_regex -lboost_date_time -lboost_chrono
PISTACHE_FLAGS = -lpistache
PTHREAD = -lpthread
all: client.o server.o

server.o: $(SVRSRC)
	$(CC) -o $@ $(SVRSRC) $(PTHREAD) $(LIBS)

client.o: $(CLISRC)
	$(CC) -o $@ $(CLISRC) $(LIBS) $(CFLAGS) $(PTHREAD) $(BOOST_FLAGS)
	
clean:
	$(RM) -r gen-cpp
	$(RM) $(TARGET)