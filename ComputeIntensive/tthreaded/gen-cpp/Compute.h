/**
 * Autogenerated by Thrift Compiler (0.14.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Compute_H
#define Compute_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include <memory>
#include "compute_types.h"



#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class ComputeIf {
 public:
  virtual ~ComputeIf() {}
  virtual void computed_value(const int32_t row, const int32_t column) = 0;
};

class ComputeIfFactory {
 public:
  typedef ComputeIf Handler;

  virtual ~ComputeIfFactory() {}

  virtual ComputeIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(ComputeIf* /* handler */) = 0;
};

class ComputeIfSingletonFactory : virtual public ComputeIfFactory {
 public:
  ComputeIfSingletonFactory(const ::std::shared_ptr<ComputeIf>& iface) : iface_(iface) {}
  virtual ~ComputeIfSingletonFactory() {}

  virtual ComputeIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(ComputeIf* /* handler */) {}

 protected:
  ::std::shared_ptr<ComputeIf> iface_;
};

class ComputeNull : virtual public ComputeIf {
 public:
  virtual ~ComputeNull() {}
  void computed_value(const int32_t /* row */, const int32_t /* column */) {
    return;
  }
};

typedef struct _Compute_computed_value_args__isset {
  _Compute_computed_value_args__isset() : row(false), column(false) {}
  bool row :1;
  bool column :1;
} _Compute_computed_value_args__isset;

class Compute_computed_value_args {
 public:

  Compute_computed_value_args(const Compute_computed_value_args&);
  Compute_computed_value_args& operator=(const Compute_computed_value_args&);
  Compute_computed_value_args() : row(0), column(0) {
  }

  virtual ~Compute_computed_value_args() noexcept;
  int32_t row;
  int32_t column;

  _Compute_computed_value_args__isset __isset;

  void __set_row(const int32_t val);

  void __set_column(const int32_t val);

  bool operator == (const Compute_computed_value_args & rhs) const
  {
    if (!(row == rhs.row))
      return false;
    if (!(column == rhs.column))
      return false;
    return true;
  }
  bool operator != (const Compute_computed_value_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Compute_computed_value_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Compute_computed_value_pargs {
 public:


  virtual ~Compute_computed_value_pargs() noexcept;
  const int32_t* row;
  const int32_t* column;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Compute_computed_value_result {
 public:

  Compute_computed_value_result(const Compute_computed_value_result&);
  Compute_computed_value_result& operator=(const Compute_computed_value_result&);
  Compute_computed_value_result() {
  }

  virtual ~Compute_computed_value_result() noexcept;

  bool operator == (const Compute_computed_value_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Compute_computed_value_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Compute_computed_value_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Compute_computed_value_presult {
 public:


  virtual ~Compute_computed_value_presult() noexcept;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class ComputeClient : virtual public ComputeIf {
 public:
  ComputeClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ComputeClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void computed_value(const int32_t row, const int32_t column);
  void send_computed_value(const int32_t row, const int32_t column);
  void recv_computed_value();
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class ComputeProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::std::shared_ptr<ComputeIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (ComputeProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_computed_value(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  ComputeProcessor(::std::shared_ptr<ComputeIf> iface) :
    iface_(iface) {
    processMap_["computed_value"] = &ComputeProcessor::process_computed_value;
  }

  virtual ~ComputeProcessor() {}
};

class ComputeProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  ComputeProcessorFactory(const ::std::shared_ptr< ComputeIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::std::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::std::shared_ptr< ComputeIfFactory > handlerFactory_;
};

class ComputeMultiface : virtual public ComputeIf {
 public:
  ComputeMultiface(std::vector<std::shared_ptr<ComputeIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~ComputeMultiface() {}
 protected:
  std::vector<std::shared_ptr<ComputeIf> > ifaces_;
  ComputeMultiface() {}
  void add(::std::shared_ptr<ComputeIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void computed_value(const int32_t row, const int32_t column) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->computed_value(row, column);
    }
    ifaces_[i]->computed_value(row, column);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class ComputeConcurrentClient : virtual public ComputeIf {
 public:
  ComputeConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(prot);
  }
  ComputeConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
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
  void computed_value(const int32_t row, const int32_t column);
  int32_t send_computed_value(const int32_t row, const int32_t column);
  void recv_computed_value(const int32_t seqid);
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