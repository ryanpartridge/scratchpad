/*
 * DirectorImpl.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio/spawn.hpp>

#include <DirectorImpl.hpp>

DirectorImpl::DirectorImpl() :
    endpoint_(boost::asio::ip::address_v4::loopback(), 9876),
    acceptor_(io_service_)
{

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
    boost::system::error_code ec;
    while (!ec)
    {
        boost::shared_ptr<boost::asio::ip::tcp::socket> listen_socket = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(io_service_));
        acceptor_.async_accept(*listen_socket, yield[ec]);
        if (!ec)
        {
//            boost::asio::spawn(io_service_, boost::bind(&handle_connection, listen_socket, boost::ref(io_service), _1));
        }
    }
}

std::string DirectorImpl::getValue(const std::string& name)
{
    std::string value = "hello";
    return value;
}
