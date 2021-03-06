/**
 * Autogenerated by Thrift Compiler (0.9.2)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef MessageService_H
#define MessageService_H

#include <thrift/TDispatchProcessor.h>
#include "sample_types.h"

namespace sample {

class MessageServiceIf {
 public:
  virtual ~MessageServiceIf() {}
  virtual void getMessage(Message& _return, const int32_t key) = 0;
  virtual void setMessage(const Message& message) = 0;
};

class MessageServiceIfFactory {
 public:
  typedef MessageServiceIf Handler;

  virtual ~MessageServiceIfFactory() {}

  virtual MessageServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(MessageServiceIf* /* handler */) = 0;
};

class MessageServiceIfSingletonFactory : virtual public MessageServiceIfFactory {
 public:
  MessageServiceIfSingletonFactory(const boost::shared_ptr<MessageServiceIf>& iface) : iface_(iface) {}
  virtual ~MessageServiceIfSingletonFactory() {}

  virtual MessageServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(MessageServiceIf* /* handler */) {}

 protected:
  boost::shared_ptr<MessageServiceIf> iface_;
};

class MessageServiceNull : virtual public MessageServiceIf {
 public:
  virtual ~MessageServiceNull() {}
  void getMessage(Message& /* _return */, const int32_t /* key */) {
    return;
  }
  void setMessage(const Message& /* message */) {
    return;
  }
};

typedef struct _MessageService_getMessage_args__isset {
  _MessageService_getMessage_args__isset() : key(false) {}
  bool key :1;
} _MessageService_getMessage_args__isset;

class MessageService_getMessage_args {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

  MessageService_getMessage_args(const MessageService_getMessage_args&);
  MessageService_getMessage_args& operator=(const MessageService_getMessage_args&);
  MessageService_getMessage_args() : key(0) {
  }

  virtual ~MessageService_getMessage_args() throw();
  int32_t key;

  _MessageService_getMessage_args__isset __isset;

  void __set_key(const int32_t val);

  bool operator == (const MessageService_getMessage_args & rhs) const
  {
    if (!(key == rhs.key))
      return false;
    return true;
  }
  bool operator != (const MessageService_getMessage_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageService_getMessage_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const MessageService_getMessage_args& obj);
};


class MessageService_getMessage_pargs {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};


  virtual ~MessageService_getMessage_pargs() throw();
  const int32_t* key;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const MessageService_getMessage_pargs& obj);
};

typedef struct _MessageService_getMessage_result__isset {
  _MessageService_getMessage_result__isset() : success(false) {}
  bool success :1;
} _MessageService_getMessage_result__isset;

class MessageService_getMessage_result {
 public:

  static const char* ascii_fingerprint; // = "0B00C6A32956D0FB910392C197F6E622";
  static const uint8_t binary_fingerprint[16]; // = {0x0B,0x00,0xC6,0xA3,0x29,0x56,0xD0,0xFB,0x91,0x03,0x92,0xC1,0x97,0xF6,0xE6,0x22};

  MessageService_getMessage_result(const MessageService_getMessage_result&);
  MessageService_getMessage_result& operator=(const MessageService_getMessage_result&);
  MessageService_getMessage_result() {
  }

  virtual ~MessageService_getMessage_result() throw();
  Message success;

  _MessageService_getMessage_result__isset __isset;

  void __set_success(const Message& val);

  bool operator == (const MessageService_getMessage_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const MessageService_getMessage_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageService_getMessage_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const MessageService_getMessage_result& obj);
};

typedef struct _MessageService_getMessage_presult__isset {
  _MessageService_getMessage_presult__isset() : success(false) {}
  bool success :1;
} _MessageService_getMessage_presult__isset;

class MessageService_getMessage_presult {
 public:

  static const char* ascii_fingerprint; // = "0B00C6A32956D0FB910392C197F6E622";
  static const uint8_t binary_fingerprint[16]; // = {0x0B,0x00,0xC6,0xA3,0x29,0x56,0xD0,0xFB,0x91,0x03,0x92,0xC1,0x97,0xF6,0xE6,0x22};


  virtual ~MessageService_getMessage_presult() throw();
  Message* success;

  _MessageService_getMessage_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const MessageService_getMessage_presult& obj);
};

typedef struct _MessageService_setMessage_args__isset {
  _MessageService_setMessage_args__isset() : message(false) {}
  bool message :1;
} _MessageService_setMessage_args__isset;

class MessageService_setMessage_args {
 public:

  static const char* ascii_fingerprint; // = "0A23AF95FD017F7C6C78570E7E17112A";
  static const uint8_t binary_fingerprint[16]; // = {0x0A,0x23,0xAF,0x95,0xFD,0x01,0x7F,0x7C,0x6C,0x78,0x57,0x0E,0x7E,0x17,0x11,0x2A};

  MessageService_setMessage_args(const MessageService_setMessage_args&);
  MessageService_setMessage_args& operator=(const MessageService_setMessage_args&);
  MessageService_setMessage_args() {
  }

  virtual ~MessageService_setMessage_args() throw();
  Message message;

  _MessageService_setMessage_args__isset __isset;

  void __set_message(const Message& val);

  bool operator == (const MessageService_setMessage_args & rhs) const
  {
    if (!(message == rhs.message))
      return false;
    return true;
  }
  bool operator != (const MessageService_setMessage_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageService_setMessage_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const MessageService_setMessage_args& obj);
};


class MessageService_setMessage_pargs {
 public:

  static const char* ascii_fingerprint; // = "0A23AF95FD017F7C6C78570E7E17112A";
  static const uint8_t binary_fingerprint[16]; // = {0x0A,0x23,0xAF,0x95,0xFD,0x01,0x7F,0x7C,0x6C,0x78,0x57,0x0E,0x7E,0x17,0x11,0x2A};


  virtual ~MessageService_setMessage_pargs() throw();
  const Message* message;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const MessageService_setMessage_pargs& obj);
};


class MessageService_setMessage_result {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  MessageService_setMessage_result(const MessageService_setMessage_result&);
  MessageService_setMessage_result& operator=(const MessageService_setMessage_result&);
  MessageService_setMessage_result() {
  }

  virtual ~MessageService_setMessage_result() throw();

  bool operator == (const MessageService_setMessage_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const MessageService_setMessage_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MessageService_setMessage_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const MessageService_setMessage_result& obj);
};


class MessageService_setMessage_presult {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~MessageService_setMessage_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const MessageService_setMessage_presult& obj);
};

class MessageServiceClient : virtual public MessageServiceIf {
 public:
  MessageServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  MessageServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void getMessage(Message& _return, const int32_t key);
  void send_getMessage(const int32_t key);
  void recv_getMessage(Message& _return);
  void setMessage(const Message& message);
  void send_setMessage(const Message& message);
  void recv_setMessage();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class MessageServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<MessageServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (MessageServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_getMessage(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_setMessage(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  MessageServiceProcessor(boost::shared_ptr<MessageServiceIf> iface) :
    iface_(iface) {
    processMap_["getMessage"] = &MessageServiceProcessor::process_getMessage;
    processMap_["setMessage"] = &MessageServiceProcessor::process_setMessage;
  }

  virtual ~MessageServiceProcessor() {}
};

class MessageServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  MessageServiceProcessorFactory(const ::boost::shared_ptr< MessageServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< MessageServiceIfFactory > handlerFactory_;
};

class MessageServiceMultiface : virtual public MessageServiceIf {
 public:
  MessageServiceMultiface(std::vector<boost::shared_ptr<MessageServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~MessageServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<MessageServiceIf> > ifaces_;
  MessageServiceMultiface() {}
  void add(boost::shared_ptr<MessageServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void getMessage(Message& _return, const int32_t key) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->getMessage(_return, key);
    }
    ifaces_[i]->getMessage(_return, key);
    return;
  }

  void setMessage(const Message& message) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->setMessage(message);
    }
    ifaces_[i]->setMessage(message);
  }

};

} // namespace

#endif
