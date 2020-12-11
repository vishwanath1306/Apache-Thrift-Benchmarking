TARGET = client server
IDL = hello.thrift
SVRSRC = echonb_server.cpp gen-cpp/HelloWorld.cpp
CLISRC = echonb_client.cpp gen-cpp/HelloWorld.cpp

CC = g++
LIBS = -lthrift -lthriftnb -levent -lpthread
CFLAGS = -Wall -std=c++11
THRIFT = thrift -gen cpp

all: client server

gen-cpp/Message.cpp: $(IDL)
	$(THRIFT) $(IDL)

server: $(SVRSRC)
	$(CC) -o $@ $(SVRSRC) $(CFLAGS) $(LIBS)

client: $(CLISRC)
	$(CC) -o $@ $(CLISRC) $(CFLAGS) $(LIBS)
	
clean:
	$(RM) -r gen-cpp
	$(RM) $(TARGET)
