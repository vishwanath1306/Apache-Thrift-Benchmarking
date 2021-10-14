/**
 * Autogenerated by Thrift Compiler (0.15.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef UniqueIDAsync_H
#define UniqueIDAsync_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include <memory>
#include "uniqueidasync_types.h"



#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class UniqueIDAsyncIf {
 public:
  virtual ~UniqueIDAsyncIf() {}
  virtual void compute_unique_id(const int64_t id) = 0;
};

class UniqueIDAsyncIfFactory {
 public:
  typedef UniqueIDAsyncIf Handler;

  virtual ~UniqueIDAsyncIfFactory() {}

  virtual UniqueIDAsyncIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(UniqueIDAsyncIf* /* handler */) = 0;
  };

class UniqueIDAsyncIfSingletonFactory : virtual public UniqueIDAsyncIfFactory {
 public:
  UniqueIDAsyncIfSingletonFactory(const ::std::shared_ptr<UniqueIDAsyncIf>& iface) : iface_(iface) {}
  virtual ~UniqueIDAsyncIfSingletonFactory() {}

  virtual UniqueIDAsyncIf* getHandler(const ::apache::thrift::TConnectionInfo&) override {
    return iface_.get();
  }
  virtual void releaseHandler(UniqueIDAsyncIf* /* handler */) override {}

 protected:
  ::std::shared_ptr<UniqueIDAsyncIf> iface_;
};

class UniqueIDAsyncNull : virtual public UniqueIDAsyncIf {
 public:
  virtual ~UniqueIDAsyncNull() {}
  void compute_unique_id(const int64_t /* id */) override {
    return;
  }
};

typedef struct _UniqueIDAsync_compute_unique_id_args__isset {
  _UniqueIDAsync_compute_unique_id_args__isset() : id(false) {}
  bool id :1;
} _UniqueIDAsync_compute_unique_id_args__isset;

class UniqueIDAsync_compute_unique_id_args {
 public:

  UniqueIDAsync_compute_unique_id_args(const UniqueIDAsync_compute_unique_id_args&) noexcept;
  UniqueIDAsync_compute_unique_id_args& operator=(const UniqueIDAsync_compute_unique_id_args&) noexcept;
  UniqueIDAsync_compute_unique_id_args() noexcept
                                       : id(0) {
  }

  virtual ~UniqueIDAsync_compute_unique_id_args() noexcept;
  int64_t id;

  _UniqueIDAsync_compute_unique_id_args__isset __isset;

  void __set_id(const int64_t val);

  bool operator == (const UniqueIDAsync_compute_unique_id_args & rhs) const
  {
    if (!(id == rhs.id))
      return false;
    return true;
  }
  bool operator != (const UniqueIDAsync_compute_unique_id_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UniqueIDAsync_compute_unique_id_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class UniqueIDAsync_compute_unique_id_pargs {
 public:


  virtual ~UniqueIDAsync_compute_unique_id_pargs() noexcept;
  const int64_t* id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class UniqueIDAsyncClient : virtual public UniqueIDAsyncIf {
 public:
  UniqueIDAsyncClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  UniqueIDAsyncClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void compute_unique_id(const int64_t id) override;
  void send_compute_unique_id(const int64_t id);
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class UniqueIDAsyncProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::std::shared_ptr<UniqueIDAsyncIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext) override;
 private:
  typedef  void (UniqueIDAsyncProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_compute_unique_id(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  UniqueIDAsyncProcessor(::std::shared_ptr<UniqueIDAsyncIf> iface) :
    iface_(iface) {
    processMap_["compute_unique_id"] = &UniqueIDAsyncProcessor::process_compute_unique_id;
  }

  virtual ~UniqueIDAsyncProcessor() {}
};

class UniqueIDAsyncProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  UniqueIDAsyncProcessorFactory(const ::std::shared_ptr< UniqueIDAsyncIfFactory >& handlerFactory) noexcept :
      handlerFactory_(handlerFactory) {}

  ::std::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo) override;

 protected:
  ::std::shared_ptr< UniqueIDAsyncIfFactory > handlerFactory_;
};

class UniqueIDAsyncMultiface : virtual public UniqueIDAsyncIf {
 public:
  UniqueIDAsyncMultiface(std::vector<std::shared_ptr<UniqueIDAsyncIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~UniqueIDAsyncMultiface() {}
 protected:
  std::vector<std::shared_ptr<UniqueIDAsyncIf> > ifaces_;
  UniqueIDAsyncMultiface() {}
  void add(::std::shared_ptr<UniqueIDAsyncIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void compute_unique_id(const int64_t id) override {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->compute_unique_id(id);
    }
    ifaces_[i]->compute_unique_id(id);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class UniqueIDAsyncConcurrentClient : virtual public UniqueIDAsyncIf {
 public:
  UniqueIDAsyncConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(prot);
  }
  UniqueIDAsyncConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
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
  void compute_unique_id(const int64_t id) override;
  void send_compute_unique_id(const int64_t id);
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