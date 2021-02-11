#include "server_event_handler.cpp"				
#include "gen-cpp/UniqueID.h"
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/server/TServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <memory>
#include <iostream>
#include <string>
#include <thread>
#include <functional>
#include <unistd.h>
#include <cstdlib>
#include <mutex>
#include <sstream>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift;
using namespace ::apache::thrift::concurrency;

using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;


using std::make_shared;
using std::shared_ptr;

#define CUSTOM_EPOCH 1514764800000

static int64_t CURRENT_TIMESTAMP = -1;
static int counter = 0;


static int get_counter(int64_t timestamp){
        if (CURRENT_TIMESTAMP > timestamp) {
                std::cout << "Timestamps are not incremental.";
                exit(EXIT_FAILURE);
        }
  if (CURRENT_TIMESTAMP == timestamp) {
    return counter++;
  } else {
    CURRENT_TIMESTAMP = timestamp;
    counter = 0;
    return counter++;
  }
}

u_int16_t HashMacAddressPid(const std::string &mac)
{
  u_int16_t hash = 0;
  std::string mac_pid = mac + std::to_string(getpid());
  for ( unsigned int i = 0; i < mac_pid.size(); i++ ) {
    hash += ( mac[i] << (( i & 1 ) * 8 ));
  }
  return hash;
}

int GetMachineId (std::string *mac_hash) {
  std::string mac;
  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP );
  if ( sock < 0 ) {
    std::cout << "Unable to obtain MAC address";
    return -1;
  }

  struct ifconf conf{};
  char ifconfbuf[ 128 * sizeof(struct ifreq)  ];
  memset( ifconfbuf, 0, sizeof( ifconfbuf ));
  conf.ifc_buf = ifconfbuf;
  conf.ifc_len = sizeof( ifconfbuf );
  if ( ioctl( sock, SIOCGIFCONF, &conf ))
  {
    std::cout << "Unable to obtain MAC address";
    return -1;
  }

  struct ifreq* ifr;
  for (
      ifr = conf.ifc_req;
      reinterpret_cast<char *>(ifr) <
          reinterpret_cast<char *>(conf.ifc_req) + conf.ifc_len;
      ifr++) {
    if ( ifr->ifr_addr.sa_data == (ifr+1)->ifr_addr.sa_data ) {
      continue;  // duplicate, skip it
    }

    if ( ioctl( sock, SIOCGIFFLAGS, ifr )) {
      continue;  // failed to get flags, skip it
    }
    if ( ioctl( sock, SIOCGIFHWADDR, ifr ) == 0 ) {
      mac = std::string(ifr->ifr_addr.sa_data);
      if (!mac.empty()) {
        break;
      }
    }
  }
  close(sock);

  std::stringstream stream;
  stream << std::hex << HashMacAddressPid(mac);
  *mac_hash = stream.str();

  if (mac_hash->size() > 3) {
    mac_hash->erase(0, mac_hash->size() - 3);
  } else if (mac_hash->size() < 3) {
    *mac_hash = std::string(3 - mac_hash->size(), '0') + *mac_hash;
  }
  return 0;
}


class UniqueIDHandler: public UniqueIDIf{

    public:

        virtual void compute_unique_id(std::string& _return, const int32_t id){

                // int32_t modified_value = id * 10;
                
                std::mutex thread_lock;

                thread_lock.lock();

                int64_t timestamp = duration_cast<milliseconds>(
                        system_clock::now().time_since_epoch()).count() - CUSTOM_EPOCH;
                int64_t counter_value = get_counter(timestamp);
                thread_lock.unlock();

                std::stringstream sstream;
                sstream<< std::hex << timestamp;
                std::string timestamp_hex(sstream.str());

                if (timestamp_hex.size() > 10){
                        timestamp_hex.erase(0, timestamp_hex.size() - 10);
                }else{
                        timestamp_hex = std::string(10 - timestamp_hex.size(), '0') + timestamp_hex;
                }

                sstream.clear();
                sstream.str(std::string());

                sstream << std::hex << counter_value;
                std::string counter_hex(sstream.str());
                
                if(counter_hex.size() > 3){
                        counter_hex.erase(0, counter_hex.size() - 3);
                }else{
                        counter_hex = std::string( 3 - counter_hex.size(), '0') + counter_hex;
                }

                std::string machine_id;

                int64_t machine_id_str = GetMachineId(&machine_id);

                std::string unique_id_str = machine_id + timestamp_hex + counter_hex;
                int64_t post_id = stoul(unique_id_str, nullptr, 16) & 0x7FFFFFFFFFFFFFFF;

                std::string unique_id_string = std::to_string(post_id);
                // std::cout<<"This is great"<<machine_id_str;
                // std::string new_value = std::to_string(modified_value);
                _return = unique_id_string;
        }
};

class UniqueIDHandlerFactory: public UniqueIDIfFactory{


        public:
                UniqueIDHandlerFactory() {;}

                virtual UniqueIDIf* getHandler(const TConnectionInfo& conn){
                        return new UniqueIDHandler();
                }

                virtual void releaseHandler(UniqueIDIf* handler){
                        delete handler;
                }

};


int main(){

        const int port = 3068;
        auto handler = make_shared<UniqueIDHandler>();
        auto proc = make_shared<UniqueIDProcessor>(handler);
        auto trans_svr = make_shared<TServerSocket>(port);

        auto trans_fac = make_shared<TBufferedTransportFactory>();
        auto proto_fac = make_shared<TCompactProtocolFactory>();

        auto t_man = ThreadManager::newSimpleThreadManager(1,8);
        auto t_fac = make_shared<ThreadFactory>();
        t_man->threadFactory(t_fac);
        t_man->start();

        TThreadPoolServer server(proc, trans_svr, trans_fac, proto_fac, t_man);
        server.setServerEventHandler(make_shared<PoolSvrEvtHandler>(t_man,1,8));
        server.serve();
        
        return 0;
}