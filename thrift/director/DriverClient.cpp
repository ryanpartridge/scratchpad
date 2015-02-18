/*
 * DriverClient.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */

#include <DriverClient.hpp>
#include <thrift/Thrift.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <boost/make_shared.hpp>

namespace c4
{
    DriverClient::DriverClient(const std::string& socketPath) :
        path_(socketPath),
        socket_(boost::make_shared<apache::thrift::transport::TSocket>(path_)),
        transport_(boost::make_shared<apache::thrift::transport::TBufferedTransport>(socket_)),
        protocol_(boost::make_shared<apache::thrift::protocol::TBinaryProtocol>(transport_)),
        client_(boost::make_shared<DriverServiceClient>(protocol_))
    {
    }

    DriverClient::~DriverClient()
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

    void DriverClient::initialize()
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

    std::string DriverClient::doAction(const int driverId)
    {
        std::string result;
        try
        {
            std::cout << "calling doAction: " << driverId << std::endl;
            client_->doAction(result, driverId);
        }
        catch (apache::thrift::TException& te)
        {
        }
        return result;
    }
} /* namespace c4 */
