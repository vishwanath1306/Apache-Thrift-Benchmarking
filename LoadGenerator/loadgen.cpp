#include "dsb-gen-cpp/UniqueID.h"
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <memory>
#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <map>
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

std::mutex outstanding_lock_mutex;
int64_t curr_outstanding = 0;
std::atomic_bool run_workload(true); 


// Implement a server in the client side. 

void init_logging(std::string latency_log_file){
    
    logging::add_file_log(
        
        keywords::file_name=latency_log_file,
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name=latency_log_file,
        keywords::format = "[%TimeStamp%] [%ThreadID%] %Message%",
        keywords::auto_flush = true
        );

        logging::add_common_attributes();
}

void time_to_run(int64_t sleep_time){
    for(int i=0; i<sleep_time; i++){
        std::this_thread::sleep_for(std::chrono::seconds(sleep_time));    
    }
    
    run_workload = false;
}


std::map<TTransport, UniqueIDClient> get_client_transport(std::string host, int64_t port, int64_t server_type){

    if (server_type == 1){
        shared_ptr<TTransport> trans;
        trans = make_shared<TSocket>(host, port);
        trans = make_shared<TFramedTransport>(trans);
        auto proto = make_shared<TJSONProtocol>(trans);
        UniqueIDClient client(proto);
        std::map<TTransport, UniqueIDClient> return_data = {
            {trans, client}
        };
        return return_data;
    }

}


void closed_loop_generator(int64_t outstanding_req, std::string host, int64_t port, int64_t server_type){

        // shared_ptr<TTransport> trans;
        
        
        // if(server_type == 1){
        //     shared_ptr<TTransport> trans;
        //     trans = make_shared<TSocket>(host, port);
        //     trans = make_shared<TFramedTransport>(trans);
        //     auto proto = make_shared<TJSONProtocol>(trans);
        //     UniqueIDClient client(proto);
        // }

        std::map<TTransport, UniqueIDClient> client_detail = get_client_transport(host, port, 1);

        
        while(run_workload){
            std::string msg("processed");

            for(int i=0; i<outstanding_req; i++){
                // Have multiple threads which call compute_unique_id
                client.compute_unique_id(msg, 100);
                outstanding_lock_mutex.lock();
                curr_outstanding++;
                outstanding_lock_mutex.unlock();                
            }

        }
        trans->close();
}



int main(int argc, char* argv[]){

    int outstanding = atoi(argv[1]);
    std::string host = argv[2];
    int port = atoi(argv[3]);
    std::string log_file = argv[4];

    std::thread run_workload_thread(time_to_run, 5);
    std::thread workload_gen(closed_loop_generator, outstanding, host, port);

    init_logging(log_file);

    run_workload_thread.join();
    workload_gen.join();

}