TARGET = client server
IDL = hello.thrift
SVRSRC = compute_server.cpp gen-cpp/Compute.cpp
CLISRC = compute_client.cpp gen-cpp/Compute.cpp

CC = g++
LIBS = -lthrift
CFLAGS = -Wall -std=c++17 
THRIFT = thrift -gen cpp
# BOOST_FLAGS = -lboost_log
BOOST_FLAGS	= -DBOOST_LOG_DYN_LINK -I$(HOME)/include -I/usr/include -L$(HOME)/lib -lboost_log_setup -lboost_log -lboost_filesystem -lboost_system -lboost_thread -lboost_regex -lboost_date_time -lboost_chrono
PISTACHE_FLAGS = -lpistache
PTHREAD = -pthread
TORCH_FLAGS = -I/libtorch/include -L/libtorch/lib -g -lstdc++ -pthread -ltorch_cpu -lc10
all: client.o server.o

gen-cpp/Message.cpp: $(IDL)
	$(THRIFT) $(IDL)

server.o: $(SVRSRC)
	$(CC) -o $@ $(SVRSRC) $(LIBS) $(TORCH_FLAGS)

client.o: $(CLISRC)
	$(CC) -o $@ $(CLISRC) $(LIBS) $(PISTACHE_FLAGS) $(PTHREAD) $(BOOST_FLAGS)
	
clean:
	$(RM) -r gen-cpp
	$(RM) $(TARGET)