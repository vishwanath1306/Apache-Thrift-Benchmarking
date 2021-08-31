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

        keywords::file_name="mlinfer_ttd_wrk_loadgen_1.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="mlinfer_ttd_wrk_loadgen_1.log",
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


void run_workload_gen(int64_t seconds, int64_t reqps){


    shared_ptr<TTransport> trans;

}