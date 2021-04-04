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
#include <pistache/endpoint.h>

#define BOOST_LOG_DYN_LINK 1

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace Pistache;
using std::shared_ptr;
using std::make_shared;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;


void init_logging(std::string filename){
    logging::add_file_log(
        
        keywords::file_name=filename,
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name=filename,
        keywords::format = "[%TimeStamp%]  [%ThreadID%] %Message%"
        );

        logging::add_common_attributes();
}

class RESTProxyHandler: public Http::Handler{

    private:
        shared_ptr<TBufferedTransport> unique_transport;
        shared_ptr<UniqueIDClient> unique_client;

    public:

        HTTP_PROTOTYPE(RESTProxyHandler)

        RESTProxyHandler(shared_ptr<TBufferedTransport> transport_param, shared_ptr<UniqueIDClient>& client_param){
            unique_transport = transport_param;
            unique_client = client_param;
        }

        void onRequest(const Http::Request& request, Http::ResponseWriter response){
            if(!unique_transport->isOpen()){
                unique_transport->open();
            }

            std::string mono;
            unique_client->compute_unique_id(mono, 100);
            std::cout<<"The ID is "<<mono;
            response.send(Http::Code::Ok, mono);
        }
};

int main(int argc, char* argv[]){

    shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 3068));
    shared_ptr<TBufferedTransport> transport(new TBufferedTransport(socket));
    shared_ptr<TCompactProtocol> protocol(new TCompactProtocol(transport));
    shared_ptr<UniqueIDClient> unique_id_client(new UniqueIDClient(protocol));
    
    Address addr(Ipv4::any(), 9082);
    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint rest_server(addr);
    rest_server.init();
    rest_server.setHandler(make_shared<RESTProxyHandler>(transport, unique_id_client));
    std::thread rest_thread([&](){
        rest_server.serve();
    });
    
    std::cout<<"Started server at port 9082"<<std::endl;
    rest_thread.join();
}

// int main(int argc, char* argv[]){

//     init_logging(argv[3]);
    
//     shared_ptr<TTransport> trans;
//     trans = make_shared<TSocket>("localhost", 3068);
//     trans = make_shared<TBufferedTransport>(trans);
//     auto proto = make_shared<TCompactProtocol>(trans);
//     UniqueIDClient client(proto);

//     int64_t seconds = atoi(argv[1]);
//     int64_t counter = 0;
//     int64_t reqps = atoi(argv[2]);
//     // std::cout<<"Here"<<std::endl;
//     trans->open();
//     while(true){

//         if(counter == seconds){
//             break;
//         }
        
//         for(int64_t i = 1; i <= reqps; i++){
//             std::string input;

//             auto start = std::chrono::high_resolution_clock::now();

//             client.compute_unique_id(input, 100);
//             auto elapsed = std::chrono::high_resolution_clock::now() - start;
//             long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
//             BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Microseconds): "<< microseconds;
//         }
//         std::cout<<"Sent out "<<reqps<<" requests for "<<counter<<" seconds"<<std::endl;
//         sleep(1);
//         counter++;
//     }

//     trans->close();
// }