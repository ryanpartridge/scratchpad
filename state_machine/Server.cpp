/*
 * Server.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/make_shared.hpp>

#include <Server.hpp>

Server::Server(boost::asio::io_service& io_service) :
    mainService_(io_service),
    pushTimer_(io_service),
    signal_(io_service),
    running_(false)
{
    std::cout << "[Server] constructor" << std::endl;
}

Server::~Server()
{
    std::cout << "[Server] destructor" << std::endl;
    stop();
}

void Server::start()
{
    std::cout << "[Server] start" << std::endl;
    signal_.add(SIGTERM);
    signal_.async_wait(boost::bind(&Server::handleSignal, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::signal_number));
    pushTimer_.expires_from_now(boost::chrono::seconds(2));
    machine_ = boost::make_shared<StateMachine>(boost::ref(workerService_), shared_from_this());
    workerService_.post(boost::bind(&StateMachine::start, boost::ref(machine_)));
    machineThread_ = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, boost::ref(workerService_)));
    pushTimer_.async_wait(boost::bind(&Server::handlePushTimer, shared_from_this(), "hello", boost::asio::placeholders::error));
    running_ = true;
}

void Server::stop()
{
    if (running_)
    {
        std::cout << "[Server] stop" << std::endl;
        pushTimer_.cancel();
        signal_.cancel();

        // tell the machine to stop and give it time to finish up
        machine_->stop();
        workerService_.stop();
        if (!machineThread_->try_join_for(boost::chrono::seconds(1)))
        {
            std::cout << "[Server] interrupting machine thread" << std::endl;
            machineThread_->interrupt();
            std::cout << "[Server] waiting on machine thread" << std::endl;
            machineThread_->try_join_for(boost::chrono::seconds(5));
        }
        machine_.reset();
        machineThread_.reset();
        running_ = false;
    }
}

void Server::handlePushTimer(const std::string message, const boost::system::error_code& ec)
{
    if (!ec)
    {
        std::cout << "[Server] pushing value to state machine" << std::endl;
        machine_->pushMessage(message);
        if (message != "stop")
        {
            pushTimer_.expires_from_now(boost::chrono::seconds(5));
            pushTimer_.async_wait(boost::bind(&Server::handlePushTimer, shared_from_this(), "stop", boost::asio::placeholders::error));
        }
    }
}

void Server::handleSignal(const boost::system::error_code& ec, boost::uint32_t signal)
{
    if (!ec)
    {
        std::cout << "[Server] handling signal number: " << signal << std::endl;
        stop();
    }
}
