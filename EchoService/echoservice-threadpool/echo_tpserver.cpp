#include "server_event_handler.cpp"				
#include "gen-cpp/HelloWorld.h"
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/server/TServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <memory>
#include <iostream>
#include <string>
#include <thread>
#include <functional>

using namespace ::apache::thrift;				
using namespace ::apache::thrift::protocol;		
using namespace ::apache::thrift::transport; 		
using namespace ::apache::thrift::server; 			
using namespace ::apache::thrift::concurrency; 		
using std::shared_ptr; 					
using std::make_shared; 

class HelloWorldHandler: public HelloWorldIf{

    public:
        virtual void hello_world(std::string& _return) override{
            
            std::thread::id curr_thread = std::this_thread::get_id();

            std::cout<< "The thread is "<< curr_thread <<std::endl;
            sleep(1);
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
    
    const int port = 3047;
    auto handler = make_shared<HelloWorldHandler>();			
    auto proc = make_shared<HelloWorldProcessor>(handler);		
    auto trans_svr = make_shared<TServerSocket>(port);		

  //Setup the protocol and layered transport factories		
    auto trans_fac = make_shared<TBufferedTransportFactory>();
    auto proto_fac = make_shared<TCompactProtocolFactory>();

  //Setup the thread manager and thread factory, then create the threads 
    auto t_man = ThreadManager::newSimpleThreadManager(2,1);		
    auto t_fac = make_shared<ThreadFactory>();
    t_man->threadFactory(t_fac);  						
    t_man->start(); 					

    TThreadPoolServer server(proc, trans_svr, trans_fac, proto_fac, t_man);
    server.setServerEventHandler(make_shared<PoolSvrEvtHandler>(t_man,2,4));
    server.serve();
}