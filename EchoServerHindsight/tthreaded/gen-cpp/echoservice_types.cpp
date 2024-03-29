/**
 * Autogenerated by Thrift Compiler (0.17.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "echoservice_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace echoservice {


TraceContext::~TraceContext() noexcept {
}


void TraceContext::__set_req_id(const int64_t val) {
  this->req_id = val;
}

void TraceContext::__set_baggage(const std::string& val) {
  this->baggage = val;
}
std::ostream& operator<<(std::ostream& out, const TraceContext& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t TraceContext::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->req_id);
          this->__isset.req_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->baggage);
          this->__isset.baggage = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t TraceContext::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("TraceContext");

  xfer += oprot->writeFieldBegin("req_id", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->req_id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("baggage", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->baggage);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(TraceContext &a, TraceContext &b) {
  using ::std::swap;
  swap(a.req_id, b.req_id);
  swap(a.baggage, b.baggage);
  swap(a.__isset, b.__isset);
}

TraceContext::TraceContext(const TraceContext& other0) {
  req_id = other0.req_id;
  baggage = other0.baggage;
  __isset = other0.__isset;
}
TraceContext& TraceContext::operator=(const TraceContext& other1) {
  req_id = other1.req_id;
  baggage = other1.baggage;
  __isset = other1.__isset;
  return *this;
}
void TraceContext::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "TraceContext(";
  out << "req_id=" << to_string(req_id);
  out << ", " << "baggage=" << to_string(baggage);
  out << ")";
}

} // namespace
