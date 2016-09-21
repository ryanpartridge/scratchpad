/*
 * DirectorImpl.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: rpartridge
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>

#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>

#include <DirectorImpl.hpp>

DirectorImpl::DirectorImpl() :
    endpoint_(boost::asio::ip::address_v4::loopback(), 9876),
    acceptor_(io_service_),
    inQueue_(QueueOwner::get_in_queue()),
    outQueue_(QueueOwner::get_out_queue()),
    driver_(io_service_)
{
    // force the queue fd to be created
    outQueue_.eventFd();
}

DirectorImpl::~DirectorImpl()
{

}

void DirectorImpl::start()
{
    std::cout << "starting director" << std::endl;
    boost::system::error_code ec;
    startAcceptor(ec);

    if (!ec)
    {
        io_service_.run();
    }
}

void DirectorImpl::startAcceptor(boost::system::error_code& ec)
{
    std::cout << "setting up the acceptor socket" << std::endl;
    acceptor_.open(endpoint_.protocol(), ec);
    if (ec)
    {
        return;
    }

    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec)
    {
        return;
    }

    acceptor_.bind(endpoint_, ec);
    if (ec)
    {
        return;
    }

    acceptor_.listen(128, ec);
    if (ec)
    {
        return;
    }

    boost::asio::spawn(io_service_, boost::bind(&DirectorImpl::acceptConnection, this, _1));
}

void DirectorImpl::acceptConnection(boost::asio::yield_context yield)
{
    std::cout << "listening for connections" << std::endl;
    boost::system::error_code ec;
    while (!ec)
    {
        boost::shared_ptr<boost::asio::ip::tcp::socket> connection = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(io_service_));
        acceptor_.async_accept(*connection, yield[ec]);
        if (!ec)
        {
            boost::asio::spawn(io_service_, boost::bind(&DirectorImpl::handleConnection, this, connection, _1));
        }
    }
}

void DirectorImpl::handleConnection(boost::shared_ptr<boost::asio::ip::tcp::socket> connection, boost::asio::yield_context yield)
{
    std::cout << "got a connection" << std::endl;
    boost::asio::spawn(io_service_, boost::bind(&DirectorImpl::serviceOutQueue, this, connection, _1));

    boost::system::error_code ec;
    boost::asio::streambuf buffer;
    std::size_t bytes_read = 0;
    while ((bytes_read = boost::asio::async_read_until(*connection, buffer, "\r\n", yield[ec])) > 0 && !ec)
    {
        boost::asio::streambuf::const_buffers_type buffers = buffer.data();
        std::string data(boost::asio::buffers_begin(buffers), boost::asio::buffers_begin(buffers) + bytes_read);
        buffer.consume(bytes_read);
        boost::algorithm::trim(data);
        std::cout << "incoming message: " << data << std::endl;
        if (boost::algorithm::istarts_with(data, "req|"))
        {
            boost::asio::spawn(yield, boost::bind(&DirectorImpl::dispatchRequest, this, data, _1));
        }
        else if (boost::algorithm::istarts_with(data, "resp|"))
        {
            inQueue_.push(data);
        }
    }
    std::cout << "connection closed" << std::endl;
}

void DirectorImpl::serviceOutQueue(boost::shared_ptr<boost::asio::ip::tcp::socket> connection, boost::asio::yield_context yield)
{
    std::cout << "watching outQueue_ for connection" << std::endl;
    boost::system::error_code ec;
    int outFd = outQueue_.eventFd();
    if (outFd == -1)
    {
        std::cout << "error getting the event file descriptor" << std::endl;
        return;
    }

    boost::asio::posix::stream_descriptor descriptor(io_service_, outFd);
    std::size_t bytes_read = 0;
    ec = boost::system::error_code();

    std::cout << "attempting to read the outQueue_ file descriptor" << std::endl;
    std::size_t eventCount;
    boost::asio::mutable_buffers_1 buffer(&eventCount, sizeof(eventCount));
    while ((bytes_read = boost::asio::async_read(descriptor, buffer, yield[ec])) > 0 && !ec)
    {
        std::cout << eventCount << " items in the out queue" << std::endl;
        std::string payload;
        while (outQueue_.front(payload))
        {
            boost::asio::async_write(*connection, boost::asio::buffer(payload.data(), payload.size()), yield[ec]);
            outQueue_.pop();
        }
        eventCount = 0;
    }

    std::cout << "releasing outQueue_ file descriptor" << std::endl;
    descriptor.release();
    std::cout << "done servicing outQueue_" << std::endl;
}

void DirectorImpl::dispatchRequest(const std::string& request, boost::asio::yield_context yield)
{
    std::vector<std::string> msgParts;
    boost::algorithm::split(msgParts, request, boost::algorithm::is_any_of(std::string("|")), boost::algorithm::token_compress_on);
    if (msgParts.size() > 2)
    {
        std::string reqTag = msgParts[1];
        if (boost::algorithm::iequals(msgParts[2], "getValue") && msgParts.size() > 3)
        {
            std::ostringstream response;
            response << "resp|" << reqTag << "|getValue|" << getValue(msgParts[3], yield) << "\r\n";
            outQueue_.push(response.str());
        }
        if (boost::algorithm::iequals(msgParts[2], "isSet") && msgParts.size() > 3)
        {
            std::ostringstream response;
            response << "resp|" << reqTag << "|isSet|" << std::boolalpha << isSet(msgParts[3], yield) << std::noboolalpha << "\r\n";
            outQueue_.push(response.str());
        }
    }
}

std::string DirectorImpl::getValue(const std::string& name, boost::asio::yield_context yield)
{
    boost::system::error_code ec;
    std::size_t value = driver_.getCount(yield[ec]);
    if (ec)
    {
        std::cout << "error calling getCount" << std::endl;
    }
    std::cout << "answer to Driver::getCount: " << value << std::endl;
    std::ostringstream response;
    response << "Hello, " << name << " (lucky number: " << value << ")";
    return response.str();
}

bool DirectorImpl::isSet(const std::string& name, boost::asio::yield_context yield)
{
    return true;
}
