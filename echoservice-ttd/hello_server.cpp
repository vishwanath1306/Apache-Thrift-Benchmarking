#include "gen-cpp/HelloWorld.h"
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransport.h>
#include <thrift/TProcessor.h>

#include <memory>
#include <string>
#include <iostream>
#include <thread>

using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift;

using std::make_shared;
using std::shared_ptr;


class HelloWorldHandler: public HelloWorldIf{

    public:
        virtual void hello_world(std::string& _return) override{
            
            std::thread::id curr_thread = std::this_thread::get_id();

            std::cout<< "The thread is "<< curr_thread <<std::endl;
            _return.assign("This is Sparta");
        }
};

class HelloWorldHandlerFactory: public HelloWorldIfFactory{

    public: 
        HelloWorldHandlerFactory() {;}

        virtual HelloWorldIf* getHandler(const TConnectionInfo& conn){
            return new HelloWorldHandler();
        }

        virtual void releaseHandler(HelloWorldIf* handler){
            delete handler;
        }
};


int main(){
    auto handler_fac = make_shared<HelloWorldHandlerFactory>();
    auto proc_fac = make_shared<HelloWorldProcessorFactory>(handler_fac);
    
    auto transport_server = make_shared<TServerSocket>(3062);
    auto transport_factory = make_shared<TFramedTransportFactory>();
    auto protocol_factory = make_shared<TJSONProtocolFactory>();

    TThreadedServer server(proc_fac, transport_server, transport_factory, protocol_factory);
    server.serve();
}