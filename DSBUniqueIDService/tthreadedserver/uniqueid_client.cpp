#include "gen-cpp/UniqueID.h"
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <memory>
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

void init_logging(std::string filename){
    logging::add_file_log(
        
        keywords::file_name=filename,
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name=filename,
        keywords::format = "[%TimeStamp%]  [%ThreadID%] %Message%"
        );

        logging::add_common_attributes();
}


int main(int argc, char* argv[]){

    init_logging(argv[3]);
    
    shared_ptr<TTransport> trans;
    trans = make_shared<TSocket>("localhost", 3066);
    trans = make_shared<TFramedTransport>(trans);
    auto proto = make_shared<TJSONProtocol>(trans);

    int64_t seconds = atoi(argv[1]);
    int64_t counter = 0;
    int64_t reqps = atoi(argv[2]);
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
        std::cout<<"Sent out "<<reqps<<" requests for "<<counter<<" seconds"<<std::endl;
        sleep(1);
        ++counter;
    }
    
    trans->close();
    return 1;
}