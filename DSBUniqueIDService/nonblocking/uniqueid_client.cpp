#include "gen-cpp/UniqueID.h"
#include <thrift/Thrift.h>
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
using std::shared_ptr;
using std::make_shared;
using namespace Pistache;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;


void init_logging(){

    logging::add_file_log(
        
        keywords::file_name="uniqueid_nb_4096.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="uniqueid_nb_4096.log",
        keywords::format = "[%TimeStamp%]  [%ThreadID%] %Message%",
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
            // if(!unique_transport->isOpen()){
            //     unique_transport->open();
            // }

            shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 3067));
            shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
            shared_ptr<TCompactProtocol> protocol(new TCompactProtocol(transport));
            shared_ptr<UniqueIDClient> unique_id_client(new UniqueIDClient(protocol));
            
            transport->open();
            std::string mono;
            auto start = std::chrono::high_resolution_clock::now();
            unique_id_client->compute_unique_id(mono, 100);
            // unique_client->compute_unique_id(mono, 100); // Previous for spawning client. 
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            BOOST_LOG_TRIVIAL(info) << "The time to execute client (Microseconds): " << microseconds;
            // std::cout<<"The ID is "<<mono<<std::endl;
            response.send(Http::Code::Ok, mono);

        }
        
};


int main(int argc, char* argv[]){
    
    shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 3067));
    shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
    shared_ptr<TCompactProtocol> protocol(new TCompactProtocol(transport));
    shared_ptr<UniqueIDClient> unique_id_client(new UniqueIDClient(protocol));

    Address addr(Ipv4::any(), 9081);
    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint rest_server(addr);
    rest_server.init();
    rest_server.setHandler(make_shared<RESTProxyHandler>(transport, unique_id_client));
    std::thread rest_thread([&](){
        init_logging();
        rest_server.serve();
    });

    std::cout<<"Started server at port 9081"<<std::endl;
    rest_thread.join();
}