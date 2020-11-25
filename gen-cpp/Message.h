/**
 * Autogenerated by Thrift Compiler (0.14.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Message_H
#define Message_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include <memory>
#include "simple_types.h"



#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class MessageIf {
 public:
  virtual ~MessageIf() {}
  virtual void motd(std::string& _return) = 0;
};

class MessageIfFactory {
 public:
  typedef MessageIf Handler;

  virtual ~MessageIfFactory() {}

  virtual MessageIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(MessageIf* /* handler */) = 0;
};

class MessageIfSingletonFactory : virtual public MessageIfFactory {
 public:
  MessageIfSingletonFactory(const ::std::shared_ptr<MessageIf>& iface) : iface_(iface) {}
  virtual ~MessageIfSingletonFactory() {}

  virtual MessageIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(MessageIf* /* handler */) {}

 protected:
  ::std::shared_ptr<MessageIf> iface_;
};

class MessageNull : virtual public MessageIf {
 public:
  virtual ~MessageNull() {}
  void motd(std::string& /* _return */) {
    return;
  }
};


class Message_motd_args {
 public:

  Message_motd_args(const Message_motd_args&);
  Message_motd_args& operator=(const Message_motd_args&);
  Message_motd_args() {
  }

  virtual ~Message_motd_args() noexcept;

  bool operator == (const Message_motd_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Message_motd_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Message_motd_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Message_motd_pargs {
 public:


  virtual ~Message_motd_pargs() noexcept;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Message_motd_result__isset {
  _Message_motd_result__isset() : success(false) {}
  bool success :1;
} _Message_motd_result__isset;

class Message_motd_result {
 public:

  Message_motd_result(const Message_motd_result&);
  Message_motd_result& operator=(const Message_motd_result&);
  Message_motd_result() : success() {
  }

  virtual ~Message_motd_result() noexcept;
  std::string success;

  _Message_motd_result__isset __isset;

  void __set_success(const std::string& val);

  bool operator == (const Message_motd_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Message_motd_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Message_motd_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Message_motd_presult__isset {
  _Message_motd_presult__isset() : success(false) {}
  bool success :1;
} _Message_motd_presult__isset;

class Message_motd_presult {
 public:


  virtual ~Message_motd_presult() noexcept;
  std::string* success;

  _Message_motd_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class MessageClient : virtual public MessageIf {
 public:
  MessageClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  MessageClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void motd(std::string& _return);
  void send_motd();
  void recv_motd(std::string& _return);
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class MessageProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::std::shared_ptr<MessageIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (MessageProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_motd(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  MessageProcessor(::std::shared_ptr<MessageIf> iface) :
    iface_(iface) {
    processMap_["motd"] = &MessageProcessor::process_motd;
  }

  virtual ~MessageProcessor() {}
};

class MessageProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  MessageProcessorFactory(const ::std::shared_ptr< MessageIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::std::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::std::shared_ptr< MessageIfFactory > handlerFactory_;
};

class MessageMultiface : virtual public MessageIf {
 public:
  MessageMultiface(std::vector<std::shared_ptr<MessageIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~MessageMultiface() {}
 protected:
  std::vector<std::shared_ptr<MessageIf> > ifaces_;
  MessageMultiface() {}
  void add(::std::shared_ptr<MessageIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void motd(std::string& _return) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->motd(_return);
    }
    ifaces_[i]->motd(_return);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class MessageConcurrentClient : virtual public MessageIf {
 public:
  MessageConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(prot);
  }
  MessageConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void motd(std::string& _return);
  int32_t send_motd();
  void recv_motd(std::string& _return, const int32_t seqid);
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif



#endif
