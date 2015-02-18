/*
 * DirectorClient.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */

#include <DirectorClient.hpp>
#include <thrift/Thrift.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <boost/make_shared.hpp>

namespace c4
{
    DirectorClient::DirectorClient(const std::string& socketPath) :
        path_(socketPath),
        socket_(boost::make_shared<apache::thrift::transport::TSocket>(path_)),
        transport_(boost::make_shared<apache::thrift::transport::TBufferedTransport>(socket_)),
        protocol_(boost::make_shared<apache::thrift::protocol::TBinaryProtocol>(transport_)),
        client_(boost::make_shared<DirectorServiceClient>(protocol_))
    {
    }

    DirectorClient::~DirectorClient()
    {
        try
        {
            if (transport_->isOpen())
            {
                transport_->close();
            }
        }
        catch (apache::thrift::TException& te)
        {
        }
    }

    void DirectorClient::initialize()
    {
        try
        {
            if (!transport_->isOpen())
            {
                transport_->open();
            }
        }
        catch (apache::thrift::TException& te)
        {
        }
    }

    std::string DirectorClient::getValue(const int id)
    {
        std::string result;
        try
        {
            std::cout << "calling getValue: " << id << std::endl;
            client_->getValue(result, id);
        }
        catch (apache::thrift::TException& te)
        {
        }
        return result;
    }
} /* namespace c4 */
