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

#include <DriverImpl.hpp>

std::size_t DriverImpl::nextDriverId_ = 0;

DriverImpl::DriverImpl() :
    driverId_(++nextDriverId_),
    endpoint_(boost::asio::ip::address_v4::loopback(), 6789),
    acceptor_(io_service_),
    inQueue_(QueueOwner::get_in_queue()),
    outQueue_(QueueOwner::get_out_queue())
{
    // force the queue fd to be created
    int fd = outQueue_.eventFd();
    std::cout << "outQueue_ file descriptor at startup: " << fd << std::endl;
}

DriverImpl::~DriverImpl()
{
}

void DriverImpl::start()
{
    std::cout << "starting driver" << std::endl;
    boost::system::error_code ec;
    startAcceptor(ec);

    if (!ec)
    {
        io_service_.run();
    }
}

void DriverImpl::startAcceptor(boost::system::error_code& ec)
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

    boost::asio::spawn(io_service_, boost::bind(&DriverImpl::acceptConnection, this, _1));
}

void DriverImpl::acceptConnection(boost::asio::yield_context yield)
{
    std::cout << "listening for connections" << std::endl;
    boost::system::error_code ec;
    while (!ec)
    {
        boost::shared_ptr<boost::asio::ip::tcp::socket> connection = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(io_service_));
        acceptor_.async_accept(*connection, yield[ec]);
        if (!ec)
        {
            boost::asio::spawn(io_service_, boost::bind(&DriverImpl::handleConnection, this, connection, _1));
        }
    }
}

void DriverImpl::handleConnection(boost::shared_ptr<boost::asio::ip::tcp::socket> connection, boost::asio::yield_context yield)
{
    std::cout << "got a connection" << std::endl;
    boost::asio::spawn(io_service_, boost::bind(&DriverImpl::serviceOutQueue, this, connection, _1));

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

void DriverImpl::serviceOutQueue(boost::shared_ptr<boost::asio::ip::tcp::socket> connection, boost::asio::yield_context yield)
{
    std::cout << "watching outQueue_ for connection" << std::endl;
    boost::system::error_code ec;
    int outFd = outQueue_.eventFd();
    if (outFd == -1)
    {
        std::cout << "error getting the event file descriptor" << std::endl;
        return;
    }

    /*
    std::cout << "waiting before probing outQueue_" << std::endl;
    boost::asio::steady_timer timer(io_service_, boost::chrono::seconds(10));
    timer.async_wait(yield[ec]);

    std::cout << "probing outQueue_ (file descriptor " << outFd << ")" << std::endl;
*/

    boost::asio::posix::stream_descriptor descriptor(io_service_, outFd);
//    std::cout << "outQueue_ file descriptor is " << (descriptor.is_open() ? "" : "not ") << "open" << std::endl;
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
            boost::asio::async_write(*connection, boost::asio::buffer(payload.data(), payload.size()), yield[ec]);
            outQueue_.pop();
        }
        eventCount = 0;
    }

    std::cout << "releasing outQueue_ file descriptor" << std::endl;
    descriptor.release();
    std::cout << "done servicing outQueue_" << std::endl;
}

std::size_t DriverImpl::getCount()
{
    return 42;
}
