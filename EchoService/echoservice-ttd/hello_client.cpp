#include "gen-cpp/HelloWorld.h"
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <memory>
#include <iostream>
#include <string>
#include <thread>
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
        
        keywords::file_name="echoservice_ttd_100.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="echoservice_ttd_100.log",
        keywords::format = "[%TimeStamp%]  [%ThreadID%] %Message%",
        keywords::auto_flush = true
        );

        logging::add_common_attributes();
}


class RESTProxyHandler: public Http::Handler{
     
     private: 
        shared_ptr<HelloWorldClient> client;
        shared_ptr<TTransport> transport; 
     public:

        HTTP_PROTOTYPE(RESTProxyHandler);

        RESTProxyHandler(shared_ptr<HelloWorldClient>& client_param, shared_ptr<TTransport> transport_param){
            client = client_param;
            transport = transport_param;
        }

        void onRequest(const Http::Request& request, Http::ResponseWriter response){
            if(!transport->isOpen()){
                transport->open();
            }

            std::string out;
            auto start = std::chrono::high_resolution_clock::now();
            client->hello_world(out);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
            BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Microseconds): " << microseconds;
            std::cout<<out<<std::endl;
            response.send(Http::Code::Ok, out);
        }
        
};

int main(int argc, char* argv[]){

    // shared_ptr<TTransport> trans;
    // trans = make_shared<TSocket>("localhost", 3046);
    // trans = make_shared<TFramedTransport>(trans);
    // shared_ptr<TJSONProtocol> proto(new TJSONProtocol(trans));     
    // shared_ptr<HelloWorldClient> client(    );

    shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 3046));
    shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
    shared_ptr<TJSONProtocol> protocol(new TJSONProtocol(transport));
    shared_ptr<HelloWorldClient> hello_world_client(new HelloWorldClient(protocol));

    Address addr(Ipv4::any(), Port(9080));
    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint rest_server(addr);
    rest_server.init();
    rest_server.setHandler(make_shared<RESTProxyHandler>(hello_world_client, transport));
    std::thread rest_thread([&](){init_logging(); rest_server.serve();});

    std::cout <<"Started Server at port: 9080" <<std::endl;
    rest_thread.join();

    return 0;

}


// int main(int argc, char* argv[]){

//     init_logging();
    
//     shared_ptr<TTransport> trans;
//     trans = make_shared<TSocket>("localhost", 3046);
//     trans = make_shared<TFramedTransport>(trans);
//     auto proto = make_shared<TJSONProtocol>(trans);

//     int64_t seconds = atoi(argv[1]);
//     int64_t counter = 0;

//     HelloWorldClient client(proto);

//     trans->open();
//     while(true){
//         if(counter == seconds){
//             break;
//         }
//         for(int i=0; i<10; i++){
//             std::string msg;
//             auto start = std::chrono::high_resolution_clock::now();
//             client.hello_world(msg);
//             // std::cout << msg << std::endl;
//             auto elapsed = std::chrono::high_resolution_clock::now() - start;
//             long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
//             BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Microseconds): "<< microseconds;
//             sleep(0.1);
//             // TODO: Create a timerfd object, and conver this for all the files. 
//         }
//         std::cout<<"Finished processing for "<<counter<<" seconds"<<std::endl;
//         counter++;
//     }
//     trans->close();
// }