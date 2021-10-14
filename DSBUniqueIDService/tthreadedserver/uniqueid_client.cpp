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
        
        keywords::file_name="uniqueid_ttd_multiple_1000.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="uniqueid_ttd_multiple_1000.log",
        keywords::format = "[%TimeStamp%] [%ThreadID%] %Message%",
        keywords::auto_flush = true
        );

        logging::add_common_attributes();
}


class RESTProxyHandler: public Http::Handler{

    private: 

        shared_ptr<TTransport> unique_transport;
        shared_ptr<UniqueIDClient> unique_client;
    
    public: 

        HTTP_PROTOTYPE(RESTProxyHandler)

        RESTProxyHandler(shared_ptr<TTransport> transport_param, shared_ptr<UniqueIDClient>& client_param){

            unique_transport = transport_param;
            unique_client = client_param;
        }

        void onRequest(const Http::Request& request, Http::ResponseWriter response){
            
            shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 3066));
            shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
            shared_ptr<TJSONProtocol> protocol(new TJSONProtocol(transport));
            shared_ptr<UniqueIDClient> unique_id_client(new UniqueIDClient(protocol));

            transport->open();

            std::string mono;
            // auto start = std::chrono::high_resolution_clock::now();
            unique_id_client->compute_unique_id(mono, 100);
            // auto elapsed = std::chrono::high_resolution_clock::now() - start;
            // long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            // BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Microseconds): "<< microseconds;            
            transport->close();
            response.send(Http::Code::Ok, mono);

        }


};


int main(int argc, char* argv[]){

    shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 3066));
    shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
    shared_ptr<TJSONProtocol> protocol(new TJSONProtocol(transport));
    shared_ptr<UniqueIDClient> unique_id_client(new UniqueIDClient(protocol));

    Address addr("0.0.0.0", 9080);
    auto opts = Http::Endpoint::options().threads(10);
    Http::Endpoint rest_server(addr);
    rest_server.init();
    rest_server.setHandler(make_shared<RESTProxyHandler>(transport, unique_id_client));
    std::thread rest_thread([&](){
        init_logging();
        rest_server.serve();
    });

    std::cout<<"Started server at port 9080"<<std::endl;
    rest_thread.join();

}
