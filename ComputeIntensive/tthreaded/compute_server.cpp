#include "gen-cpp/Compute.h"
#include "gen-cpp/compute_types.h"
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
#include <cstdlib>

using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift;
using std::make_shared;
using std::shared_ptr;


void matrix_multiplication(int32_t row, int32_t column){

    srand(time(0));

    int array_1[row][column];
    int array_2[column][row];

    int i, j;
    for( i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            array_1[i][j] = rand();
        }
    }

    for( i = 0; i < column; i++){
        for(j = 0; j < row; j++){
            array_2[i][j] = rand();
        }
    }

    int multiplied[row][row];

    memset(multiplied, 0, sizeof(multiplied[0][0]) * row * row);

    for(i = 0; i < row; ++i)
        for(j = 0; j < column; ++j)
            for(int k = 0; k < column; ++k)
            {
                multiplied[i][j] += array_1[i][k] * array_2[k][j];
            }


}

class ComputeHandler: public ComputeIf {

    public:

    virtual void computed_value(const int32_t row, const int32_t column) override{

        std::cout<<"Matrix values are "<<row<<" "<<column<<std::endl;
        matrix_multiplication(row, column);
        std::cout<<"Finished computation"<<std::endl;
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
    
    auto transport_server = make_shared<TServerSocket>(3065);
    auto transport_factory = make_shared<TFramedTransportFactory>();
    auto protocol_factory = make_shared<TJSONProtocolFactory>();

    TThreadedServer server(proc_fac, transport_server, transport_factory, protocol_factory);
    server.serve();
}