TARGET = client
IDL = uniqueid.thrift
LOAD_CLI = loadgen.cpp dsb-gen-cpp/UniqueID.cpp

CC = g++
LIBS = -lthrift
CFLAGS = -Wall -std=c++17
BOOST_FLAGS	= -DBOOST_LOG_DYN_LINK -I$(HOME)/include -I/usr/include -L$(HOME)/lib -lboost_log_setup -lboost_log -lboost_filesystem -lboost_system -lboost_thread -lboost_regex -lboost_date_time -lboost_chrono
PTHREAD = -pthread

all: loadgen.o

loadgen.o: $(LOAD_CLI)
	$(CC) -o $@ $(LOAD_CLI) $(LIBS) $(CFLAGS) $(PISTACHE_FLAGS) $(PTHREAD) $(BOOST_FLAGS)
	
clean:
	$(RM) -r gen-cpp
	$(RM) $(TARGET)