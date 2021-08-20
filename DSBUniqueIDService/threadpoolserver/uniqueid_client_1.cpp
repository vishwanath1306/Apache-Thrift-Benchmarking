#include "gen-cpp/UniqueID.h"
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <memory>
#include <iostream>
#include <string>
#include <thread>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#define BOOST_LOG_DYN_LINK 1

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using std::shared_ptr;
using std::make_shared;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;


void init_logging(){
    logging::add_file_log(
        
        keywords::file_name="uniqueid_tp_multi_client.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="uniqueid_tp_multi_client.log",
        keywords::format = "[%TimeStamp%] [%ThreadID%] %Message%",
        keywords::auto_flush = true
        );

        logging::add_common_attributes();
}

void run_workload_gen(int64_t seconds, int64_t reqps){
    
    shared_ptr<TTransport> trans;
    trans = make_shared<TSocket>("localhost", 3068);
    trans = make_shared<TBufferedTransport>(trans);
    auto proto = make_shared<TCompactProtocol>(trans);
    UniqueIDClient client(proto);

    // int64_t seconds = atoi(argv[1]);
    int64_t counter = 0;
    // int64_t reqps = atoi(argv[2]);
    // std::cout<<"Here"<<std::endl;
    trans->open();
    while(true){

        if(counter == seconds){
            break;
        }
        
        for(int64_t i = 1; i <= reqps; i++){
            std::string input;

            auto start = std::chrono::high_resolution_clock::now();

            client.compute_unique_id(input, 100);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Microseconds): "<< microseconds;
        }
        // std::cout<<"Sent out "<<reqps<<" requests for "<<counter<<" seconds"<<std::endl;
        sleep(1);
        counter++;
    }

    trans->close();
}


int main(int argc, char* argv[]){
    init_logging();

    int64_t seconds = atoi(argv[1]);
    int64_t reqps = atoi(argv[2]);
    int64_t no_of_clients = atoi(argv[3]);

    std::vector<std::thread> threads;

    for(int i=0 ; i < no_of_clients; i++){
        std::cout<<"Spawned Client: "<< i + 1<<std::endl;
        threads.push_back(std::thread(run_workload_gen, seconds, reqps));
    }

    for(int i=0; i < no_of_clients; i++){
        threads[i].join();
    }
}