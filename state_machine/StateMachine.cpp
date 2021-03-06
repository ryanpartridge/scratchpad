/*
 * StateMachine.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: ryan
 */

#include <iostream>
#include <boost/smart_ptr.hpp>

#include <StateMachine.hpp>
#include <IdleState.hpp>

StateMachine::StateMachine(boost::asio::io_service& io_service, boost::shared_ptr<Server> server) :
    service_(io_service),
    //work_(io_service),
    server_(server),
    running_(false)
{
    std::cout << "[StateMachine] constructor" << std::endl;
}

StateMachine::~StateMachine()
{
    std::cout << "[StateMachine] destructor" << std::endl;
    stop();
}

void StateMachine::start()
{
    std::cout << "[StateMachine] start" << std::endl;
    boost::shared_ptr<State> s = boost::shared_ptr<State>(new IdleState(*this));
    service_.post(boost::bind(&State::execute, s));
    running_ = true;
}

void StateMachine::stop()
{
    if (running_)
    {
        std::cout << "[StateMachine] stop" << std::endl;
        running_ = false;
    }
}

boost::asio::io_service& StateMachine::io_service()
{
    return service_;
}

void StateMachine::pushMessage(const std::string& message)
{
    boost::mutex::scoped_lock lock(mutex_);
    std::cout << "[StateMachine] pushing: " << message << std::endl;
    message_ = message;
    messageWaiting_.notify_all();
}

const std::string StateMachine::popMessage()
{
    boost::mutex::scoped_lock lock(mutex_);
    std::string msg;
    while (message_.empty())
    {
        std::cout << "[StateMachine] waiting for a message" << std::endl;
        messageWaiting_.wait(lock);
    }
    std::cout << "[StateMachine] popping: " << message_ << std::endl;
    msg = message_;
    message_ = "";
    return msg;
}

void StateMachine::handleStateComplete(boost::shared_ptr<State> nextState)
{
    std::cout << "[StateMachine] current state complete" << message_ << std::endl;
    if (nextState)
    {
        service_.post(boost::bind(&State::execute, nextState));
    }
}
