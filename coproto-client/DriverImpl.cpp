/*
 * DriverImpl.cpp
 *
 *  Created on: Sep 19, 2016
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>

#include <DriverImpl.hpp>

std::size_t DriverImpl::nextDriverId_ = 0;

DriverImpl::DriverImpl() :
    driverId_(++nextDriverId_),
    endpoint_(boost::asio::ip::address_v4::loopback(), 9876),
    socket_(io_service_),
    inQueue_(QueueOwner::get_in_queue()),
    outQueue_(QueueOwner::get_out_queue())
{
    // force the queue fd to be created
    outQueue_.eventFd();
}

DriverImpl::~DriverImpl()
{
}

void DriverImpl::start()
{
    std::cout << "starting driver" << std::endl;
    boost::system::error_code ec;
    boost::asio::spawn(io_service_, boost::bind(&DriverImpl::startConnection, this, _1));
    io_service_.run();
}

void DriverImpl::startConnection(boost::asio::yield_context yield)
{
    std::cout << "setting up connection with director" << std::endl;
    boost::system::error_code ec;
    socket_.async_connect(endpoint_, yield[ec]);

    if (ec)
    {
        std::cout << "error connecting to director" << std::endl;
        return;
    }
    boost::asio::spawn(io_service_, boost::bind(&DriverImpl::handleIncoming, this, _1));
    boost::asio::spawn(io_service_, boost::bind(&DriverImpl::serviceOutQueue, this, _1));
    boost::asio::spawn(io_service_, boost::bind(&DriverImpl::startRequestTimer, this, _1));
}

void DriverImpl::handleIncoming(boost::asio::yield_context yield)
{
    boost::system::error_code ec;
    boost::asio::streambuf buffer;
    std::size_t bytes_read = 0;
    while ((bytes_read = boost::asio::async_read_until(socket_, buffer, "\r\n", yield[ec])) > 0 && !ec)
    {
        boost::asio::streambuf::const_buffers_type buffers = buffer.data();
        std::string data(boost::asio::buffers_begin(buffers), boost::asio::buffers_begin(buffers) + bytes_read);
        buffer.consume(bytes_read);
        boost::algorithm::trim(data);
        std::cout << "incoming message: " << data << std::endl;
        if (boost::algorithm::istarts_with(data, "req|"))
        {
            std::vector<std::string> msgParts;
            boost::algorithm::split(msgParts, data, boost::algorithm::is_any_of(std::string("|")), boost::algorithm::token_compress_on);
            if (msgParts.size() > 2)
            {
                std::string reqTag = msgParts[1];
                if (boost::algorithm::iequals(msgParts[2], "getCount"))
                {
                    std::ostringstream response;
                    response << "resp|" << reqTag << "|getCount|" << getCount() << "\r\n";
                    outQueue_.push(response.str());
                }
            }
        }
        else if (boost::algorithm::istarts_with(data, "resp|"))
        {
            inQueue_.push(data);
        }
    }
    std::cout << "connection closed" << std::endl;
}

void DriverImpl::serviceOutQueue(boost::asio::yield_context yield)
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
        std::cout << eventCount << " items in the queue" << std::endl;
        std::string payload;
        while (outQueue_.front(payload))
        {
            boost::asio::async_write(socket_, boost::asio::buffer(payload.data(), payload.size()), yield[ec]);
            outQueue_.pop();
        }
        eventCount = 0;
    }

    std::cout << "releasing outQueue_ file descriptor" << std::endl;
    descriptor.release();
    std::cout << "done servicing outQueue_" << std::endl;
}

void DriverImpl::startRequestTimer(boost::asio::yield_context yield)
{
    boost::system::error_code ec;
    boost::asio::steady_timer timer(io_service_, boost::chrono::seconds(5));
    timer.async_wait(yield[ec]);
    if (ec)
    {
        std::cout << "timer error" << std::endl;
    }
}

std::size_t DriverImpl::getCount()
{
    return 42;
}
