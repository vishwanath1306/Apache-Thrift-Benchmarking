#include <iostream>
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>
#include "gen-cpp/AsyncService.h"


using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using std::make_shared;
using std::shared_ptr;

int main(int argc, char* argv[]){


    shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 3076));
    shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
    shared_ptr<TJSONProtocol> protocol(new TJSONProtocol(transport));
    shared_ptr<AsyncService::AsyncServiceClient> async_client(new AsyncService::AsyncServiceClient(protocol));


    transport->open();

    async_client->compute_result(100, 200);

    std::cout<<"Closing Client"<<std::endl;
    transport->close();


}