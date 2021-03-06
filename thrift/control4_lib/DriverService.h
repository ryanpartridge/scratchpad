/**
 * Autogenerated by Thrift Compiler (0.9.2)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef DriverService_H
#define DriverService_H

#include <thrift/TDispatchProcessor.h>
#include "control4_types.h"

namespace c4 {

class DriverServiceIf {
 public:
  virtual ~DriverServiceIf() {}
  virtual void doAction(std::string& _return, const int32_t driverId) = 0;
};

class DriverServiceIfFactory {
 public:
  typedef DriverServiceIf Handler;

  virtual ~DriverServiceIfFactory() {}

  virtual DriverServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(DriverServiceIf* /* handler */) = 0;
};

class DriverServiceIfSingletonFactory : virtual public DriverServiceIfFactory {
 public:
  DriverServiceIfSingletonFactory(const boost::shared_ptr<DriverServiceIf>& iface) : iface_(iface) {}
  virtual ~DriverServiceIfSingletonFactory() {}

  virtual DriverServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(DriverServiceIf* /* handler */) {}

 protected:
  boost::shared_ptr<DriverServiceIf> iface_;
};

class DriverServiceNull : virtual public DriverServiceIf {
 public:
  virtual ~DriverServiceNull() {}
  void doAction(std::string& /* _return */, const int32_t /* driverId */) {
    return;
  }
};

typedef struct _DriverService_doAction_args__isset {
  _DriverService_doAction_args__isset() : driverId(false) {}
  bool driverId :1;
} _DriverService_doAction_args__isset;

class DriverService_doAction_args {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

  DriverService_doAction_args(const DriverService_doAction_args&);
  DriverService_doAction_args& operator=(const DriverService_doAction_args&);
  DriverService_doAction_args() : driverId(0) {
  }

  virtual ~DriverService_doAction_args() throw();
  int32_t driverId;

  _DriverService_doAction_args__isset __isset;

  void __set_driverId(const int32_t val);

  bool operator == (const DriverService_doAction_args & rhs) const
  {
    if (!(driverId == rhs.driverId))
      return false;
    return true;
  }
  bool operator != (const DriverService_doAction_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DriverService_doAction_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const DriverService_doAction_args& obj);
};


class DriverService_doAction_pargs {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};


  virtual ~DriverService_doAction_pargs() throw();
  const int32_t* driverId;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const DriverService_doAction_pargs& obj);
};

typedef struct _DriverService_doAction_result__isset {
  _DriverService_doAction_result__isset() : success(false) {}
  bool success :1;
} _DriverService_doAction_result__isset;

class DriverService_doAction_result {
 public:

  static const char* ascii_fingerprint; // = "9A73381FEFD6B67F432E717102246330";
  static const uint8_t binary_fingerprint[16]; // = {0x9A,0x73,0x38,0x1F,0xEF,0xD6,0xB6,0x7F,0x43,0x2E,0x71,0x71,0x02,0x24,0x63,0x30};

  DriverService_doAction_result(const DriverService_doAction_result&);
  DriverService_doAction_result& operator=(const DriverService_doAction_result&);
  DriverService_doAction_result() : success() {
  }

  virtual ~DriverService_doAction_result() throw();
  std::string success;

  _DriverService_doAction_result__isset __isset;

  void __set_success(const std::string& val);

  bool operator == (const DriverService_doAction_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const DriverService_doAction_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DriverService_doAction_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const DriverService_doAction_result& obj);
};

typedef struct _DriverService_doAction_presult__isset {
  _DriverService_doAction_presult__isset() : success(false) {}
  bool success :1;
} _DriverService_doAction_presult__isset;

class DriverService_doAction_presult {
 public:

  static const char* ascii_fingerprint; // = "9A73381FEFD6B67F432E717102246330";
  static const uint8_t binary_fingerprint[16]; // = {0x9A,0x73,0x38,0x1F,0xEF,0xD6,0xB6,0x7F,0x43,0x2E,0x71,0x71,0x02,0x24,0x63,0x30};


  virtual ~DriverService_doAction_presult() throw();
  std::string* success;

  _DriverService_doAction_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const DriverService_doAction_presult& obj);
};

class DriverServiceClient : virtual public DriverServiceIf {
 public:
  DriverServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  DriverServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void doAction(std::string& _return, const int32_t driverId);
  void send_doAction(const int32_t driverId);
  void recv_doAction(std::string& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class DriverServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<DriverServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (DriverServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_doAction(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  DriverServiceProcessor(boost::shared_ptr<DriverServiceIf> iface) :
    iface_(iface) {
    processMap_["doAction"] = &DriverServiceProcessor::process_doAction;
  }

  virtual ~DriverServiceProcessor() {}
};

class DriverServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  DriverServiceProcessorFactory(const ::boost::shared_ptr< DriverServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< DriverServiceIfFactory > handlerFactory_;
};

class DriverServiceMultiface : virtual public DriverServiceIf {
 public:
  DriverServiceMultiface(std::vector<boost::shared_ptr<DriverServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~DriverServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<DriverServiceIf> > ifaces_;
  DriverServiceMultiface() {}
  void add(boost::shared_ptr<DriverServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void doAction(std::string& _return, const int32_t driverId) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->doAction(_return, driverId);
    }
    ifaces_[i]->doAction(_return, driverId);
    return;
  }

};

} // namespace

#endif
