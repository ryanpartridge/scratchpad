/*
 * Server.hpp
 *
 *  Created on: Sep 5, 2013
 *      Author: rpartridge
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <boost/cstdint.hpp>

#include <StateMachine.hpp>

class Server :
    public boost::enable_shared_from_this<Server>
{
public:
    Server(boost::asio::io_service& io_service);
    virtual ~Server();

    void start();
    void stop();

private:
    void handlePushTimer(const std::string message, const boost::system::error_code& ec);
    void handleSignal(const boost::system::error_code& ec, boost::uint32_t signal);

    boost::asio::io_service& mainService_;
    boost::asio::io_service workerService_;
    boost::asio::steady_timer pushTimer_;
    boost::shared_ptr<StateMachine> machine_;
    boost::shared_ptr<boost::thread> machineThread_;
    boost::asio::signal_set signal_;
    bool running_;
};

#endif /* SERVER_HPP_ */
