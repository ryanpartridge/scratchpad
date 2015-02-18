/*
 * DirectorServer.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */

#include "DirectorServer.hpp"

#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TThreadedServer.h>
#include <boost/make_shared.hpp>
#include <boost/chrono.hpp>

namespace c4
{
    DirectorServer::DirectorServer(const std::string& socketPath, const std::string& clientPath) :
        path_(socketPath),
        handler_(boost::make_shared<DirectorServiceHandler>(clientPath)),
        processor_(boost::make_shared<DirectorServiceProcessor>(handler_)),
        transport_(boost::make_shared<apache::thrift::transport::TServerSocket>(path_)),
        transportFactory_(boost::make_shared<apache::thrift::transport::TBufferedTransportFactory>()),
        protocolFactory_(boost::make_shared<apache::thrift::protocol::TBinaryProtocolFactory>())
    {
    }

    DirectorServer::~DirectorServer()
    {
        stop();
    }

    void DirectorServer::start()
    {
        if (!serverThread_)
        {
            if (!server_)
            {
                server_ = boost::make_shared<apache::thrift::server::TThreadedServer>(processor_, transport_, transportFactory_, protocolFactory_);
            }
            serverThread_ = boost::make_shared<boost::thread>(&apache::thrift::server::TServer::serve, server_);
        }
    }

    void DirectorServer::stop()
    {
        try
        {
            if (server_)
            {
                server_->stop();
                if (serverThread_)
                {
                    if (!serverThread_->try_join_for(boost::chrono::seconds(2)))
                    {
                        serverThread_->interrupt();
                        serverThread_->try_join_for(boost::chrono::seconds(2));
                    }
                    serverThread_.reset();
                }
                server_.reset();
            }
        }
        catch (apache::thrift::TException& te)
        {
        }
        catch (...)
        {
        }
    }
} /* namespace c4 */
