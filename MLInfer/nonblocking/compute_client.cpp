#include "gen-cpp/Compute.h"
#include <thrift/Thrift.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <memory>
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

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using std::shared_ptr;
using std::make_shared;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

void init_logging(){
    logging::add_file_log(
        
        keywords::file_name="mlinfer_nonblocking.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="mlinfer_nonblocking.log",
        keywords::format = "[%TimeStamp%]  [%ThreadID%] %Message%"
    );

    logging::add_common_attributes();
}
// 3057

int main(int argc, char* argv[]){
    init_logging();

    auto trans_ep = make_shared<TSocket>("localhost", 3057);
    auto trans = make_shared<TFramedTransport>(trans_ep);
    auto proto = make_shared<TCompactProtocolT<TFramedTransport>>(trans);

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