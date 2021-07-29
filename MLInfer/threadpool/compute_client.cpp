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
#include <pistache/endpoint.h>
#define BOOST_LOG_DYN_LINK 1

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace Pistache; 
using std::shared_ptr;
using std::make_shared;

namespace loggigng = boost::log;
namespace keywords = boost::log::keywords;

void init_logging(){
    loggigng::add_file_log(
        keywords::file_name="mlinfer_tp_500.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="mlinfer_tp_500.log",
        keywords::format = "[%TimeStamp%]  [%ThreadID%] %Message%"
    );
    loggigng::add_common_attributes();
}

class RESTProxyHandler: public Http::Handler{

    private:
        shared_ptr<TBufferedTransport> mlinfer_transport;
        shared_ptr<ComputeClient> mlinfer_client;
    public: 

        HTTP_PROTOTYPE(RESTProxyHandler)

        RESTProxyHandler(shared_ptr<TBufferedTransport> transport_param, shared_ptr<ComputeClient> client_param){
            mlinfer_transport = transport_param;
            mlinfer_client = client_param;
        }

         void onRequest(const Http::Request& request, Http::ResponseWriter response){

            if(!mlinfer_transport->isOpen()){
                mlinfer_transport->open();
            }

            int z = 3, y = 224, x = 224;
            std::vector<std::vector<std::vector<double>>> random_input(z, std::vector<std::vector<double>>(y, std::vector<double>(x, 1.0)));
            
            std::string mono;
            auto start = std::chrono::high_resolution_clock::now();
            mlinfer_client->compute_list(mono, random_input);
            // mlinfer_client->computed_value(10, 10);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Microseconds): "<< microseconds;
            // std::cout << mono << std::endl;
            response.send(Http::Code::Ok, "Finished Compute");
        }
};

int main(int argc, char* argv[]){
    init_logging();


    shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 3058));
    shared_ptr<TBufferedTransport> transport(new TBufferedTransport(socket));
    shared_ptr<TCompactProtocol> protocol(new TCompactProtocol(transport));
    shared_ptr<ComputeClient> compute_client(new ComputeClient(protocol));

    Address addr(Ipv4::any(), 9082);
    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint rest_server(addr);
    rest_server.init();
    rest_server.setHandler(make_shared<RESTProxyHandler>(transport, compute_client));
    std::thread rest_thread([&](){
        rest_server.serve();
    });

    std::cout<<"Started server at port 9082"<<std::endl;
    rest_thread.join();

}