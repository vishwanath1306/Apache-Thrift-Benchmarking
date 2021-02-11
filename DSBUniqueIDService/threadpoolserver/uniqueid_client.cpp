#include "gen-cpp/UniqueID.h"
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <memory>
#include <iostream>
#include <string>
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
        
        keywords::file_name="uniqueid_threadpool.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="uniqueid_threadpool.log",
        keywords::format = "[%TimeStamp%]  [%ThreadID%] %Message%"
        );

        logging::add_common_attributes();
}

int main(){

    init_logging();
    
    shared_ptr<TTransport> trans;
    trans = make_shared<TSocket>("localhost", 3068);
    trans = make_shared<TBufferedTransport>(trans);
    auto proto = make_shared<TCompactProtocol>(trans);
    UniqueIDClient client(proto);

    // std::cout<<"Here"<<std::endl;
    try
    {
        trans->open();
        std::string input;
    
        auto start = std::chrono::high_resolution_clock::now();

        client.compute_unique_id(input, 100);
        std::cout << input << std::endl;
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

        BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Microseconds): "<< microseconds;

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    trans->close();

}