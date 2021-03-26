#include "gen-cpp/Compute.h"
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <memory>
#include <iostream>
#include <string>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <cstdlib>
#define BOOST_LOG_DYN_LINK 1

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using std::make_shared;
using std::shared_ptr;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

void init_logging(){
    logging::add_file_log(
        
        keywords::file_name="mlinfer_ttd.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="mlinfer_ttd.log",
        keywords::format = "[%TimeStamp%]  [%ThreadID%] %Message%"
        );

        logging::add_common_attributes();   
}

int main(){

    init_logging();
    
    srand(time(0));

    shared_ptr<TTransport> trans;
    trans = make_shared<TSocket>("localhost", 3056);
    trans = make_shared<TFramedTransport>(trans);
    auto proto = make_shared<TJSONProtocol>(trans);

    ComputeClient client(proto);

    trans->open();

    std::string msg;
    int32_t row = rand() % 100;
    int32_t column = rand() % 100;

    auto start = std::chrono::high_resolution_clock::now();
    client.computed_value(row, column);
    // std::cout << msg << std::endl;
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Row Column Microseconds): "<< row << " "<< column<< " "<< microseconds;
    trans->close();
}