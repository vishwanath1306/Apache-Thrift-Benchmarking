#include "gen-cpp/HelloWorld.h"
#include <thrift/TProcessor.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TNonblockingServerSocket.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/ThreadFactory.h>
#include <memory>
#include <thread>
#include <iostream>
#include <string>
#include <atomic>
#include <unistd.h>

using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;
using namespace ::apache::thrift;

using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;


class HelloWorldHandler : virtual public HelloWorldIf{
    public:
        HelloWorldHandler() {;}
        void hello_world(std::string& _return) override{

            std::cout<<"Current Thread is "<<std::this_thread::get_id()<<std::endl;
            sleep(1);
            _return.assign("Hello World");
        }
};

class HelloWorldFactory: virtual public HelloWorldIfFactory{
    public:
        HelloWorldFactory() {;}

        HelloWorldIf* getHandler(const TConnectionInfo& conn) override{
            return new HelloWorldHandler();
        }

        void releaseHandler(HelloWorldIf* handler) override {
            delete handler;
        }

};

int main(){

    auto trans = make_shared<TNonblockingServerSocket>(3062);

    auto handler_fac = make_shared<HelloWorldFactory>();
    auto processor_fac = make_shared<HelloWorldProcessorFactory>(handler_fac);
    auto protocol_fac = make_shared<TCompactProtocolFactoryT<TMemoryBuffer>>();

    auto thread_man = ThreadManager::newSimpleThreadManager(1);
    thread_man->threadFactory(make_shared<ThreadFactory>());
    thread_man->start();

    TNonblockingServer server(processor_fac, protocol_fac, trans, thread_man);
    server.setNumIOThreads(1);
    server.serve();

    return 0;
}