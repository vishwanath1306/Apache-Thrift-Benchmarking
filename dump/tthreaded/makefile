TARGET = client server
IDL = simple.thrift
SVRSRC = factory_server.cpp gen-cpp/Message.cpp
CLISRC = factory_client.cpp gen-cpp/Message.cpp

CC = g++
LIBS = -lthrift
CFLAGS = -Wall -std=c++11
THRIFT = thrift -gen cpp

all: client.o server.o

gen-cpp/Message.cpp: $(IDL)
	$(THRIFT) $(IDL)

server.o: $(SVRSRC)
	$(CC) -o $@ $(SVRSRC) $(CFLAGS) $(LIBS)

client.o: $(CLISRC)
	$(CC) -o $@ $(CLISRC) $(CFLAGS) $(LIBS)
	
clean:
	$(RM) -r gen-cpp
	$(RM) $(TARGET)
