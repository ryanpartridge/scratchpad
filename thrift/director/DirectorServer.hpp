/*
 * DirectorServer.hpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */

#ifndef DIRECTORSERVER_HPP_
#define DIRECTORSERVER_HPP_

#include <DirectorService.h>
#include <DirectorServiceHandler.hpp>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

namespace c4
{
    class DirectorServer
    {
    public:
        explicit DirectorServer(const std::string& socketPath, const std::string& clientPath);
        virtual ~DirectorServer();

        void start();
        void stop();

    private:
        std::string path_;
        boost::shared_ptr<DirectorServiceHandler> handler_;
        boost::shared_ptr<apache::thrift::TProcessor> processor_;
        boost::shared_ptr<apache::thrift::transport::TServerTransport> transport_;
        boost::shared_ptr<apache::thrift::transport::TTransportFactory> transportFactory_;
        boost::shared_ptr<apache::thrift::protocol::TProtocolFactory> protocolFactory_;
        boost::shared_ptr<apache::thrift::server::TServer> server_;

        boost::shared_ptr<boost::thread> serverThread_;
    };
} /* namespace c4 */

#endif /* DIRECTORSERVER_HPP_ */
