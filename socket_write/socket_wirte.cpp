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
#include <boost/chrono.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/algorithm/string/trim.hpp>

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

class Server :
	public boost::enable_shared_from_this<Server>,
	private boost::noncopyable
{
public:
	Server(boost::asio::io_service& io_service) :
		io_service_(io_service),
		acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 5020)),
		pendingWrites_(0),
		shutdown_(false)
	{
		std::cout << "[Server] constructor" << std::endl;
	}

	virtual ~Server()
	{
		std::cout << "[Server] destructor" << std::endl;
	}

	void start()
	{
		std::cout << "[Server] waiting for connection" << std::endl;
		socket_ = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(io_service_));
		acceptor_.async_accept(*socket_, boost::bind(&Server::handleAccept, shared_from_this(), boost::asio::placeholders::error));
	}

	void stop()
	{
		std::cout << "marking shutdown flag" << std::endl;
		//shutdown_.store(true);
		io_service_.post(boost::bind(&Server::closeImpl, shared_from_this()));
	}

private:
	void closeImpl()
	{
		std::cout << "closing socket" << std::endl;
		socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket_->close();
	}

	void handleWrite(const boost::system::error_code& ec)
	{
		pendingWrites_.fetch_sub(1);
		if (!ec)
		{
			std::cout << "successfully sent message" << std::endl;
		}
		else
		{
			std::cout << "error sending message: " << ec.message() << std::endl;
		}
		if (pendingWrites_.load() == 0 && shutdown_.load())
		{
			std::cout << "need to shut down" << std::endl;
		}
	}

	void asyncWrite(const std::string& message)
	{
		std::cout << "sending message: " << boost::trim_copy(message) << std::endl;
		pendingWrites_.fetch_add(1);
		socket_->async_send(boost::asio::buffer(message.data(), message.size()), boost::bind(&Server::handleWrite, shared_from_this(), boost::asio::placeholders::error));
	}

	void handleAccept(const boost::system::error_code& ec)
	{
		if (!ec)
		{
			std::cout << "got a connection" << std::endl;
			for (unsigned count = 0; count < 10; ++count)
			{
				std::ostringstream out;
				out << "message # " << count << std::endl;
				asyncWrite(out.str());
				if (count == 0)
				{
//					stop();
//					std::cout << "closing socket" << std::endl;
//					socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
//					socket_->close();
				}
			}
		}
	}

	boost::asio::io_service& io_service_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
	boost::atomic<unsigned> pendingWrites_;
	boost::atomic<bool> shutdown_;
};

int main(int argc, char* argv[])
{
    std::cout << "starting threads" << std::endl;
    boost::asio::io_service clientService, serverService;
    //boost::asio::io_service::work serverWork(serverService);
    boost::shared_ptr<Client> c = boost::make_shared<Client>(boost::ref(clientService));
    boost::shared_ptr<Server> s = boost::make_shared<Server>(boost::ref(serverService));
    s->start();

    boost::thread serverThread(boost::bind(&boost::asio::io_service::run, boost::ref(serverService)));
//    boost::this_thread::sleep_for(boost::chrono::seconds(5));
//    s->stop();
    serverThread.join();
    std::cout << "work all done" << std::endl;
    return 0;
}
