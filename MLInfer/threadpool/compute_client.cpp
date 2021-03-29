#include "gen-cpp/Compute.h"
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

namespace loggigng = boost::log;
namespace keywords = boost::log::keywords;

void init_logging(){
    loggigng::add_file_log(
        keywords::file_name="mlinfer_tp.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="mlinfer_tp.log",
        keywords::format = "[%TimeStamp%]  [%ThreadID%] %Message%"
    );
    loggigng::add_common_attributes();
}


int main(int argc, char* argv[]){
    init_logging();

    shared_ptr<TTransport> trans;
    trans = make_shared<TSocket>("localhost", 3058);
    trans = make_shared<TBufferedTransport>(trans);
    auto proto = make_shared<TCompactProtocol>(trans);
    ComputeClient client(proto);

    std::string msg;
    int32_t row = rand() % 100;
    int32_t column = rand() % 100;
    
    std::vector<int64_t> values;
    for(int i = 0; i < 100; i++){
        int64_t secret = rand() % 1000 + 1;
        values.push_back(secret);
    }
    
    trans->open();
    auto start = std::chrono::high_resolution_clock::now();
    client.computed_value(row, column);
    // std::cout << msg << std::endl;
    // client.compute_list(msg, values);
    // std::cout<<msg<<std::endl;
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Row Column Microseconds): "<< row << " "<< column<< " "<< microseconds;
    trans->close();

}