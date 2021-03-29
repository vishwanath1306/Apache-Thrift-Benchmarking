#include "server_event_handler.cpp"
#include "gen-cpp/Compute.h"
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
#include <unistd.h>
#include <cstdlib>
#include <mutex>
#include <sstream>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <torch/script.h>

using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift;
using namespace ::apache::thrift::concurrency;

using std::make_shared;
using std::shared_ptr;


void ml_inference(){

    torch::jit::script::Module module;

    try{
        module = torch::jit::load("./traced_resnet_model.pt");   
    }catch(const c10::Error& e){
        std::cerr << "Error loading the model" << std::endl;
    }

    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(torch::ones({1,3,224,224}));

    at::Tensor output = module.forward(inputs).toTensor();
    std::cout << output.slice(1, 0, 5) << std::endl;
    std::cout <<"Inference completed" << std::endl;

}

class ComputeHandler: public ComputeIf{

    public:
        virtual void computed_value(const int32_t row, const int32_t column){
            std::cout <<"Matrix values are "<< row << " " << column << std::endl;
            ml_inference();
        }
        virtual void compute_list(std::string& _return, const std::vector<int64_t>& values){
            _return.assign("Hello World");
        }
};

class ComputeHandlerFactory: public ComputeIfFactory {
    public:
        ComputeHandlerFactory() {;}

        virtual ComputeIf* getHandler(const TConnectionInfo& conn){
            return new ComputeHandler();
        }

        virtual void releaseHandler(ComputeIf* handler){
            delete handler;
        }
};

int main(){
    
    auto handler = make_shared<ComputeHandler>();
    auto proc = make_shared<ComputeProcessor>(handler);
    auto trans_svr = make_shared<TServerSocket>(3058);

    auto trans_fac = make_shared<TBufferedTransportFactory>();
    auto proto_fac = make_shared<TCompactProtocolFactory>();

    auto t_man = ThreadManager::newSimpleThreadManager(1,4);
    auto t_fac = make_shared<ThreadFactory>();
    t_man->threadFactory(t_fac);
    t_man->start();

    TThreadPoolServer server(proc, trans_svr, trans_fac, proto_fac, t_man);
    server.setServerEventHandler(make_shared<PoolSvrEvtHandler>(t_man,1,4));
    server.serve();

    return 0;
}