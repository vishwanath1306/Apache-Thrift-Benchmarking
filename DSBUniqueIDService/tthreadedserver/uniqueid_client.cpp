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
        
        keywords::file_name="valimai_update.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="valimai_update.log",
        keywords::format = "[%TimeStamp%]  [%ThreadID%] %Message%",
        keywords::auto_flush=true
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
            if(!unique_transport->isOpen()){
                unique_transport->open();
            }
        
            std::string mono;
            unique_client->compute_unique_id(mono, 100);
            std::cout<<"The ID is "<<mono<<std::endl;
            response.send(Http::Code::Ok, mono);
        }


};


int main(int argc, char* argv[]){

    init_logging();

    shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 3066));
    shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
    shared_ptr<TJSONProtocol> protocol(new TJSONProtocol(transport));
    shared_ptr<UniqueIDClient> unique_id_client(new UniqueIDClient(protocol));

    Address addr(Ipv4::any(), 9080);
    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint rest_server(addr);
    rest_server.init();
    rest_server.setHandler(make_shared<RESTProxyHandler>(transport, unique_id_client));
    std::thread rest_thread([&](){
        rest_server.serve();
    });

    std::cout<<"Started server at port 9080"<<std::endl;
    rest_thread.join();

}

// int main(int argc, char* argv[]){

//     init_logging(argv[3]);
    
//     shared_ptr<TTransport> trans;
//     trans = make_shared<TSocket>("localhost", 3066);
//     trans = make_shared<TFramedTransport>(trans);
//     auto proto = make_shared<TJSONProtocol>(trans);

//     int64_t seconds = atoi(argv[1]);
//     int64_t counter = 0;
//     int64_t reqps = atoi(argv[2]);
//     UniqueIDClient client(proto);

//     trans->open();

//     while(true){

//         if (counter == seconds){
//             break;
//         }

//         for(int64_t i = 0; i < reqps; i++){
//             std::string msg;
//             auto start = std::chrono::high_resolution_clock::now();
//             client.compute_unique_id(msg, 100);
//             // std::cout << msg << std::endl;
//             auto elapsed = std::chrono::high_resolution_clock::now() - start;
//             long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
//             BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Microseconds): "<< microseconds;
//         }
//         std::cout<<"Sent out "<<reqps<<" requests for "<<counter<<" seconds"<<std::endl;
//         sleep(1);
//         ++counter;
//     }
    
//     trans->close();
//     return 1;
// }