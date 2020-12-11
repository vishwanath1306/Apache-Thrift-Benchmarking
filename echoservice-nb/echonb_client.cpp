#include "gen-cpp/HelloWorld.h"
#include <thrift/Thrift.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <memory>
#include <iostream>
#include <string>

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using std::shared_ptr;
using std::make_shared;

int main(){
    auto trans_ep = make_shared<TSocket>("localhost", 3062);
    auto trans = make_shared<TFramedTransport>(trans_ep);
    auto proto = make_shared<TCompactProtocolT<TFramedTransport>>(trans);

    HelloWorldClient client(proto);
    std::cout<<"Here"<<std::endl;
    try
    {
        trans->open();
        std::string input;
        do{
            client.hello_world(input);
            std::cout<<input<<std::endl;
            std::cout<<"Enter X to quit. Anything else to continue"<<std::endl;
            std::getline(std::cin, input);
        }while (0 != input.compare("X"));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    trans->close();

}