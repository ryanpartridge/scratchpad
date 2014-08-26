/*
 * io_service_post.cpp
 *
 *  Created on: Jul 16, 2013
 *      Author: rpartridge
 */
#include <iostream>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

class Client
{
public:
	Client(boost::asio::io_service& io_service) :
		io_service_(io_service),
		socket_(io_service)
	{
		std::cout << "[Client] constructor" << std::endl;
	}

	virtual ~Client()
	{
		std::cout << "[Client] destructor" << std::endl;
	}

private:
	boost::asio::io_service& io_service_;
	boost::asio::ip::tcp::socket socket_;
};

class Server
{
public:
	Server(boost::asio::io_service& io_service) :
		io_service_(io_service)
	{

	}

	virtual ~Server()
	{

	}

private:
	boost::asio::io_service& io_service_;
};

int main(int argc, char* argv[])
{
    std::cout << "Starting threads" << std::endl;
    boost::asio::io_service client_service, server_service;
    Client c(client_service);
    Server s(server_service);
    std::cout << "Work all done" << std::endl;
    return 0;
}
