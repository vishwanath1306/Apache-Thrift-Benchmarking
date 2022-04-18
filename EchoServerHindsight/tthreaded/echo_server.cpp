#include "gen-cpp/EchoService.h"
#include "gen-cpp/echoservice_types.h"
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransport.h>
#include <thrift/TProcessor.h>

#include <memory>
#include <string>
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift;

using namespace std;
using namespace echoservice;

extern "C"{
    #include "tracer/hindsight.h"
    #include "tracer/agentapi.h"
}

class EchoServerHandler: public EchoServiceIf {
        public: 
            virtual void call(const TraceContext& req, const string &message){
                hindsight_begin(req.req_id);
                cout<<req.req_id<<endl;
                hindsight_end();
            }
};

class EchoServerHandlerFactory: public EchoServiceIfFactory{
    public:

        EchoServerHandlerFactory() {;}
        virtual EchoServiceIf* getHandler(const TConnectionInfo& conn){
            return new EchoServerHandler();
        }

        virtual void releaseHandler(EchoServiceIf* handler){
            delete handler;
        }
};

int main(){
    auto handler_fac = make_shared<EchoServerHandlerFactory>();
    auto proc_fac = make_shared<EchoServiceProcessorFactory>(handler_fac);
    
    auto transport_server = make_shared<TServerSocket>(3046);
    auto transport_factory = make_shared<TFramedTransportFactory>();
    auto protocol_factory = make_shared<TJSONProtocolFactory>();

    hindsight_init("echotthreaded");
    TThreadedServer server(proc_fac, transport_server, transport_factory, protocol_factory);
    cout<<"Starting servier at port 3046"<<endl;
    server.serve();
}