#include "gen-cpp/Compute.h"
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <memory>
#include <thread>
#include <iostream>
#include <string>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <cstdlib>
#include <vector>
#define BOOST_LOG_DYN_LINK 1


using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using std::make_shared;
using std::shared_ptr;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;


void init_logging(){

    logging::add_file_log(

        keywords::file_name="mlinfer_ttd_wrk_loadgen_3.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="mlinfer_ttd_wrk_loadgen_3.log",
        keywords::format = "[%TimeStamp%] [%ThreadID%] %Message%",
        keywords::auto_flush = true
    );

    logging::add_common_attributes();
}


std::vector<std::vector<std::vector<int64_t>>> generate_sample_array(int x, int y, int z, std::vector<std::vector<std::vector<int64_t>>> &inputs){
    
    std::vector<std::vector<std::vector<int64_t>>> random_input(z, std::vector<std::vector<int64_t>>(y, std::vector<int64_t>(x, 1)));

    std::cout<<"Contents of 3D vector is "<<std::endl;

    for(int i = 0; i < x; i++){
        
        for(int j = 0; j < y; j++){
            
            for(int k = 0; k < x; k++){
                inputs[i][j][k] = rand() % 10 + 1;
                std::cout<<inputs[i][j][k]<<" ";
            }

            std::cout<<std::endl;
        }

        std::cout<<std::endl;
    }
    
    return random_input;

}


void run_workload_gen(int64_t seconds, int64_t reqps, int thread_no){


    shared_ptr<TTransport> trans;
    trans = make_shared<TSocket>("170.140.147.116", 3056);
    trans = make_shared<TFramedTransport>(trans);
    auto proto = make_shared<TJSONProtocol>(trans);

    int64_t counter = 0;

    ComputeClient client(proto);

    trans->open();

    int z = 3, y = 224, x = 224;
    int64_t response = 0;

    while(true){
        if (counter == seconds){
            break;
        }else{
            response = 0;
        }

        for(int64_t i = 0; i < reqps; i++){
            std::string msg;
            std::vector<std::vector<std::vector<double>>> random_input(z, std::vector<std::vector<double>>(y, std::vector<double>(x, 1.0)));
            auto start = std::chrono::high_resolution_clock::now();
            client.compute_list(msg, random_input);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Microseconds): "<< microseconds; 
            std::cout<<"The message is: "<< msg << std::endl;
            if(msg == "processed"){
                response++;
            }

            std::cout<<"The response for thread: "<< thread_no << " is "<< response <<std::endl; 
        }

        ++counter;
        sleep(1);
        
    }
    trans->close();

}

int main(int argc, char* argv[]){

    init_logging();

    int64_t seconds = atoi(argv[1]);
    int64_t reqps = atoi(argv[2]);
    int64_t no_of_clients = atoi(argv[3]);

    std::vector<std::thread> threads;

    for(int i = 0 ; i < no_of_clients ; i++){

        std::cout<<"Spawned Client "<< i + 1 <<std::endl;
        threads.push_back(std::thread(run_workload_gen, seconds, reqps, i+1));
    }

    for(int i=0; i < no_of_clients; i++){
        threads[i].join();
    }
}