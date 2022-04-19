#include "gen-cpp/EchoService.h"
#include "gen-cpp/echoservice_types.h"
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <memory>
#include <iostream>
#include <string>
#include <thread>

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace echoservice;
using std::make_shared;
using std::shared_ptr;


void run_workload_gen(int64_t seconds, int64_t reqps){

    shared_ptr<TTransport> trans;
    trans = make_shared<TSocket>("localhost", 3046);
    trans = make_shared<TFramedTransport>(trans);
    auto proto = make_shared<TJSONProtocol>(trans);


    int64_t counter = 0;
    EchoServiceClient client(proto);
    trans->open();

    int64_t response = 0;

    while(true){

        if(counter == seconds){
            break;
        }else{
            response = 0;
        }

        for(int64_t i = 0; i < reqps; i++){
            auto context =  new TraceContext();
            context->req_id = std::rand();
            context->baggage = "";

            client.call(*context, "extra information");
        }
    }

    trans->close();
}

int main(int argc, char* argv[]){

    // Usage: ./client 30 10 3

    int64_t seconds = atoi(argv[1]);
    int64_t reqps = atoi(argv[2]);
    int64_t no_of_clients = atoi(argv[3]);

    std::vector<std::thread> threads;
    for(int i = 0; i < no_of_clients; i++){
        std::cout<<"Spawned Client: "<< i + 1<<std::endl;
        threads.push_back(std::thread(run_workload_gen, seconds, reqps));
    }

    for(int i = 0; i <no_of_clients; i++){
        threads[i].join();
    }

}