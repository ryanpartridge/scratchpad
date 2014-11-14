/**
 * Autogenerated by Thrift Compiler (0.9.2)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "sample_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace sample {


Message::~Message() throw() {
}


void Message::__set_key(const int32_t val) {
  this->key = val;
}

void Message::__set_value(const std::string& val) {
  this->value = val;
}

const char* Message::ascii_fingerprint = "3F5FC93B338687BC7235B1AB103F47B3";
const uint8_t Message::binary_fingerprint[16] = {0x3F,0x5F,0xC9,0x3B,0x33,0x86,0x87,0xBC,0x72,0x35,0xB1,0xAB,0x10,0x3F,0x47,0xB3};

uint32_t Message::read(::apache::thrift::protocol::TProtocol* iprot) {

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
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->key);
          this->__isset.key = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->value);
          this->__isset.value = true;
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

uint32_t Message::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("Message");

  xfer += oprot->writeFieldBegin("key", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->key);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->value);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(Message &a, Message &b) {
  using ::std::swap;
  swap(a.key, b.key);
  swap(a.value, b.value);
  swap(a.__isset, b.__isset);
}

Message::Message(const Message& other0) {
  key = other0.key;
  value = other0.value;
  __isset = other0.__isset;
}
Message& Message::operator=(const Message& other1) {
  key = other1.key;
  value = other1.value;
  __isset = other1.__isset;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const Message& obj) {
  using apache::thrift::to_string;
  out << "Message(";
  out << "key=" << to_string(obj.key);
  out << ", " << "value=" << to_string(obj.value);
  out << ")";
  return out;
}

} // namespace
