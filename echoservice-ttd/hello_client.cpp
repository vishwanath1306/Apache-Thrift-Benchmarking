#include "gen-cpp/HelloWorld.h"
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <memory>
#include <iostream>
#include <string>

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using std::make_shared;
using std::shared_ptr;

int main(){
    shared_ptr<TTransport> trans;
    trans = make_shared<TSocket>("localhost", 3062);
    trans = make_shared<TFramedTransport>(trans);
    auto proto = make_shared<TJSONProtocol>(trans);

    HelloWorldClient client(proto);

    trans->open();

    std::string msg;
    do{
        client.hello_world(msg);
        std::cout << msg << std::endl;
        std::cout <<"Enter X to quit"<<std::endl;
        std::getline(std::cin, msg);
    }while(0 != msg.compare("X"));

    trans->close();
}