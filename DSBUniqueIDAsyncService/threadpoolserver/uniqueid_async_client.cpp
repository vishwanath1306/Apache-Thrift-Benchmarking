#include "gen-cpp/UniqueIDAsync.h"
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <memory>
#include <iostream>
#include <string>


using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using std::make_shared;
using std::shared_ptr;


int main(){

    shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 3078));
    shared_ptr<TBufferedTransport> transport(new TBufferedTransport(socket));
    shared_ptr<TCompactProtocol> protocol(new TCompactProtocol(transport));
    shared_ptr<UniqueIDAsyncClient> uniqueid_async_client(new UniqueIDAsyncClient(protocol));

    transport->open();

    uniqueid_async_client->compute_unique_id(100);

    transport->close();

    return 1;
    

}