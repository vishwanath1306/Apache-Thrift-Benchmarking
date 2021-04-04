#include "gen-cpp/HelloWorld.h"
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
        
        keywords::file_name="threadpool_.log",
        keywords::open_mode=std::ios_base::app,
        keywords::target_file_name="sample_threadpool.log",
        keywords::format = "[%TimeStamp%]  [%ThreadID%] %Message%"
        );

        logging::add_common_attributes();
}

class RESTProxyHandler: public Http::Handler{

    private: 
        shared_ptr<HelloWorldClient> client;
        shared_ptr<TTransport> transport;
    
    public:

        HTTP_PROTOTYPE(RESTProxyHandler)

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

// int main(int argc, char* argv[]){

//     shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 3047));
//     shared_ptr<TBufferedTransport> transport(new TBufferedTransport(socket));
//     shared_ptr<TCompactProtocol> protocol(new TCompactProtocol(transport));
//     shared_ptr<HelloWorldClient> hello_world_client(new HelloWorldClient(protocol));

//     Address addr(Ipv4::any(), 9081);
//     auto opts = Http::Endpoint::options().threads(1);
//     Http::Endpoint rest_server(addr);

//     rest_server.init();
//     rest_server.setHandler(make_shared<RESTProxyHandler>(hello_world_client, transport));
//     std::thread rest_thread([&](){
//         init_logging();
//         rest_server.serve();
//     });

//     std::cout<<"Started the server at port 9081"<<std::endl;
//     rest_thread.join();

//     return 0;
// }


int main(){

    init_logging();
    
    shared_ptr<TTransport> trans;
    trans = make_shared<TSocket>("localhost", 3048);
    trans = make_shared<TBufferedTransport>(trans);
    auto proto = make_shared<TCompactProtocol>(trans);
    HelloWorldClient client(proto);

    // std::cout<<"Here"<<std::endl;
    trans->open();
    try
    {
        std::string input;
    
        auto start = std::chrono::high_resolution_clock::now();

        client.hello_world(input);
        // std::cout << msg << std::endl;
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

        BOOST_LOG_TRIVIAL(info) <<"The time to execute client (Microseconds): "<< microseconds;

    }
    catch(const std::exception& e)
    {   
        std::cout<<"Inside exception"<<std::endl;
        std::cerr << e.what() << '\n';
    }
    
    trans->close();

}