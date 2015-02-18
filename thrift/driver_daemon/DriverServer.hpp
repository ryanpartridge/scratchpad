/*
 * DriverServer.hpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */

#ifndef DRIVERSERVER_HPP_
#define DRIVERSERVER_HPP_

#include <DriverService.h>
#include <DriverServiceHandler.hpp>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

namespace c4
{
    class DriverServer
    {
    public:
        DriverServer(const std::string& socketPath, const std::string& clientPath);
        virtual ~DriverServer();

        void start();
        void stop();

    private:
        std::string path_;
        boost::shared_ptr<DriverServiceHandler> handler_;
        boost::shared_ptr<apache::thrift::TProcessor> processor_;
        boost::shared_ptr<apache::thrift::transport::TServerTransport> transport_;
        boost::shared_ptr<apache::thrift::transport::TTransportFactory> transportFactory_;
        boost::shared_ptr<apache::thrift::protocol::TProtocolFactory> protocolFactory_;
        boost::shared_ptr<apache::thrift::server::TServer> server_;

        boost::shared_ptr<boost::thread> serverThread_;
    };

} /* namespace c4 */

#endif /* DRIVERSERVER_HPP_ */
