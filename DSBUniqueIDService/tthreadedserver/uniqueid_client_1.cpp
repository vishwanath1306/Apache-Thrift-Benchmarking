#include "gen-cpp/UniqueID.h"
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <memory>
#include <thread>
#include <iostream>
#include <string>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#define BOOST_LOG_DYN_LINK 1

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using std::make_shared;
using std::shared_ptr;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

void init_logging(){
    
    logging::add_file_log(
        
        keywords::file_name="uniqueid_ttd_loadgen_run_1.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="uniqueid_ttd_loadgen_run_1.log",
        keywords::format = "[%TimeStamp%] [%ThreadID%] %Message%",
        keywords::auto_flush = true
        );

        logging::add_common_attributes();
}

void run_workload_gen(int64_t seconds, int64_t reqps){

    init_logging();
    
    shared_ptr<TTransport> trans;
    trans = make_shared<TSocket>("170.140.147.116", 3066);
    trans = make_shared<TFramedTransport>(trans);
    auto proto = make_shared<TJSONProtocol>(trans);

    // int64_t seconds = atoi(argv[1]);
    int64_t counter = 0;
    // int64_t reqps = atoi(argv[2]);
    UniqueIDClient client(proto);

    trans->open();

    while(true){

        if (counter == seconds){
            break;
        }

        for(int64_t i = 0; i < reqps; i++){
            std::string msg;
            auto start = std::chrono::high_resolution_clock::now();
            client.compute_unique_id(msg, 100);
            // std::cout << msg << std::endl;
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Microseconds): "<< microseconds;
        }
        // std::cout<<"Sent out "<<reqps<<" requests for "<<counter<<" seconds"<<std::endl;
        sleep(1);
        ++counter;
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