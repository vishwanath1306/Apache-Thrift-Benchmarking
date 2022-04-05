#include "gen-cpp/EchoService.h"
#include "gen-cpp/echoservice_types.h"
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <memory>
#include <iostream>
#include <string>
#include <thread>

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace echoservice;
using std::make_shared;
using std::shared_ptr;

int main(int argc, char* argv[]){

    shared_ptr<TTransport> trans;
    trans = make_shared<TSocket>("localhost", 3046);
    trans = make_shared<TFramedTransport>(trans);
    auto proto = make_shared<TJSONProtocol>(trans);

    EchoServiceClient client(proto);

    trans->open();

    auto context =  new TraceContext();
    context->req_id = 123456;
    context->baggage = "hello world";

    std::cout<<context->req_id<<" "<<context->baggage<<std::endl;
    client.call(*context, "this is fun");
    trans->close();


}