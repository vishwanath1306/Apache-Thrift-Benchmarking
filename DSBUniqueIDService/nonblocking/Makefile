TARGET = client server
IDL = uniqueid.thrift
SVRSRC = uniqueid_server.cpp gen-cpp/UniqueID.cpp
CLISRC = uniqueid_client.cpp gen-cpp/UniqueID.cpp
LOAD_CLI = uniqueid_client_1.cpp gen-cpp/UniqueID.cpp

CC = g++
LIBS = -lthrift -lthriftnb -levent -lpthread
CFLAGS = -Wall -std=c++17
THRIFT = thrift -gen cpp
# BOOST_FLAGS = -lboost_log
BOOST_FLAGS	= -DBOOST_LOG_DYN_LINK -I$(HOME)/include -I/usr/include -L$(HOME)/lib -lboost_log_setup -lboost_log -lboost_filesystem -lboost_system -lboost_thread -lboost_regex -lboost_date_time -lboost_chrono
PISTACHE_FLAGS = -lpistache
PTHREAD = -pthread
all: client.o server.o

gen-cpp/Message.cpp: $(IDL)
	$(THRIFT) $(IDL)

server.o: $(SVRSRC)
	$(CC) -o $@ $(SVRSRC) $(LIBS)

client.o: $(CLISRC)
	$(CC) -o $@ $(CLISRC) $(CFLAGS) $(LIBS) $(PISTACHE_FLAGS) $(PTHREAD) $(BOOST_FLAGS)
	
loadgen.o: $(LOAD_CLI)
	$(CC) -o $@ $(LOAD_CLI) $(CFLAGS) $(LIBS) $(PTHREAD) $(BOOST_FLAGS)

clean:
	$(RM) -r gen-cpp
	$(RM) $(TARGET)