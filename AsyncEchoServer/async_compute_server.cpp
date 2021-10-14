#include "gen-cpp/AsyncService.h"
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransport.h>
#include <thrift/TProcessor.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unistd.h>
#include <cstdlib>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;


using std::make_shared;
using std::shared_ptr;


class AsyncServiceHandler : public AsyncService::AsyncServiceIf{
    
    public:

        virtual void compute_result(const int64_t num_1, const int64_t num_2){
            
            std::cout<<"Sleeping for 5 seconds"<<std::endl;
            sleep(5);
            std::cout<<"Computed Result: "<<num_1 + num_2<<std::endl;
        }
  
};


class AsyncServiceHandlerFactory: public AsyncService::AsyncServiceIfFactory{

    public:

        AsyncServiceHandlerFactory() {;}

        virtual AsyncService::AsyncServiceIf* getHandler(const TConnectionInfo& conn){
            return new AsyncServiceHandler();
        }

        virtual void releaseHandler(AsyncService::AsyncServiceIf* handler){
            delete handler;
        }
};

int main(){

    auto handler_fac = make_shared<AsyncServiceHandlerFactory>();
    auto proc_fac = make_shared<AsyncService::AsyncServiceProcessorFactory>(handler_fac);

    auto transport_server = make_shared<TServerSocket>(3076);
    auto transport_factory = make_shared<TFramedTransportFactory>();
    auto protocol_factory = make_shared<TJSONProtocolFactory>();

    TThreadedServer server(proc_fac, transport_server, transport_factory, protocol_factory);
    std::cout<<"Starting server in port 3076"<<std::endl;
    server.serve();

}