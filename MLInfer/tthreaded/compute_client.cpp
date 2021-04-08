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
#include <pistache/endpoint.h>
#define BOOST_LOG_DYN_LINK 1

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using std::make_shared;
using std::shared_ptr;
using namespace Pistache;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

void init_logging(){
    logging::add_file_log(
        
        keywords::file_name="valimai_update.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="valimai.log",
        keywords::format = "[%TimeStamp%]  [%ThreadID%] %Message%",
        keywords::auto_flush = true
        );

        logging::add_common_attributes();   
}

class RESTProxyHandler: public Http::Handler{

    private:
        shared_ptr<TFramedTransport> mlinfer_transport;
        shared_ptr<ComputeClient> mlinfer_client;
    
    public:

        HTTP_PROTOTYPE(RESTProxyHandler)

        RESTProxyHandler(shared_ptr<TFramedTransport> transport_param, shared_ptr<ComputeClient> client_param){

            mlinfer_transport = transport_param;
            mlinfer_client = client_param;

        }

        void onRequest(const Http::Request& request, Http::ResponseWriter response){

            if(!mlinfer_transport->isOpen()){
                mlinfer_transport->open();
            }

            std::string mono;
            auto start = std::chrono::high_resolution_clock::now();
            mlinfer_client->computed_value(10,10);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Microseconds): "<< microseconds;
            std::cout << "Finished computation" << std::endl;
            response.send(Http::Code::Ok, "Finished Compute");
        }
};


int main(int argc, char* argv[]){

    shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 3056));
    shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
    shared_ptr<TJSONProtocol> protocol(new TJSONProtocol(transport));
    shared_ptr<ComputeClient> compute_client(new ComputeClient(protocol));

    Address addr(Ipv4::any(), 9080);
    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint rest_server(addr);
    rest_server.init();
    rest_server.setHandler(make_shared<RESTProxyHandler>(transport, compute_client));
    std::thread rest_thread([&] () {
        init_logging();
        rest_server.serve();
    });

    std::cout<<"Started server at port 9080"<<std::endl;
    rest_thread.join();
}