#include "gen-cpp/Compute.h"
#include "gen-cpp/compute_types.h"
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransport.h>
#include <thrift/TProcessor.h>

#include <torch/script.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <vector>

using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift;
using std::make_shared;
using std::shared_ptr;


void matrix_multiplication(int32_t row, int32_t column){

    // srand(time(0));

    // int array_1[row][column];
    // int array_2[column][row];

    // int i, j;
    // for( i = 0; i < row; i++){
    //     for(j = 0; j < column; j++){
    //         array_1[i][j] = rand();
    //     }
    // }

    // for( i = 0; i < column; i++){
    //     for(j = 0; j < row; j++){
    //         array_2[i][j] = rand();
    //     }
    // }

    // int multiplied[row][row];

    // memset(multiplied, 0, sizeof(multiplied[0][0]) * row * row);

    // for(i = 0; i < row; ++i)
    //     for(j = 0; j < column; ++j)
    //         for(int k = 0; k < column; ++k)
    //         {
    //             multiplied[i][j] += array_1[i][k] * array_2[k][j];
    //         }
    torch::jit::script::Module module;

    try {
        module = torch::jit::load("./traced_resnet_model.pt");
    }catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
    }

  std::vector<torch::jit::IValue> inputs;
  inputs.push_back(torch::ones({1, 3, 224, 224}));

  at::Tensor output = module.forward(inputs).toTensor();
  std::cout << output.slice(/*dim=*/1, /*start=*/0, /*end=*/5) << '\n';
  std::cout << "ok\n";
}

class ComputeHandler: public ComputeIf {

    public:

    virtual void computed_value(const int32_t row, const int32_t column) override{

        std::cout<<"Matrix values are "<<row<<" "<<column<<std::endl;
        matrix_multiplication(row, column);
        std::cout<<"Finished computation"<<std::endl;
    }
    virtual void compute_list(std::string& _return, const std::vector<std::vector<std::vector<double>>>& values){
        std::thread::id this_id = std::this_thread::get_id();
        std::cout<<"Current Thread is: "<<this_id<<std::endl;
        torch::jit::script::Module module;

        std::vector<std::vector<std::vector<float>>> random_input(3, std::vector<std::vector<float>>(224, std::vector<float>(224, 1.0)));


            for(int i = 0; i < 3; i++){
            
            for(int j = 0; j < 224; j++){
                
                for(int k = 0; k < 224; k++){
                    random_input[i][j][k] = (float) values[i][j][k];
                }
            }

        }


        try {
            module = torch::jit::load("./traced_resnet_model.pt");
        }catch (const c10::Error& e) {
            std::cerr << "error loading the model\n";
        }


        // std::cout<<"Entering dangerous space"<<std::endl;

        auto tensor = torch::empty({1 * 3 * 224 * 224});
        float* data = tensor.data_ptr<float>();

        for (const auto& i: random_input){
            for(const auto& j: i){
                for(const auto& k: j){
                    *data++ = k;
                }
            }
        }

        std::vector<torch::jit::IValue> inputs;

        inputs.emplace_back(tensor.resize_({1, 3, 224, 224}));
        // std::cout<< "Running inference on new data" << std::endl;
        at::Tensor output = module.forward(inputs).toTensor();
        // std::cout << output.slice(/*dim=*/1, /*start=*/0, /*end=*/5) << '\n';
        // std::cout << "ok\n";

        _return.assign("processed");
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
    auto handler_fac = make_shared<ComputeHandlerFactory>();
    auto proc_fac = make_shared<ComputeProcessorFactory>(handler_fac);
    
    auto transport_server = make_shared<TServerSocket>(3056);
    auto transport_factory = make_shared<TFramedTransportFactory>();
    auto protocol_factory = make_shared<TJSONProtocolFactory>();

    TThreadedServer server(proc_fac, transport_server, transport_factory, protocol_factory);
    server.serve();
}