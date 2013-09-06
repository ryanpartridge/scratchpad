/*
 * StateMachine.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: ryan
 */

#include <iostream>
#include <boost/smart_ptr.hpp>

#include <StateMachine.hpp>
#include <IdleTask.hpp>

StateMachine::StateMachine(boost::asio::io_service& io_service, boost::shared_ptr<Server> server) :
    service_(io_service),
    //work_(io_service),
    server_(server)
{
    std::cout << "[StateMachine] constructor" << std::endl;
}

StateMachine::~StateMachine()
{
    std::cout << "[StateMachine] destructor" << std::endl;
}

void StateMachine::start()
{
    std::cout << "[StateMachine] start" << std::endl;
    boost::shared_ptr<Task> t = boost::shared_ptr<Task>(new IdleTask(*this));
    service_.post(boost::bind(&Task::execute, t));
    service_.run();
    std::cout << "[StateMachine] run complete" << std::endl;
}

void StateMachine::stop()
{
    std::cout << "[StateMachine] stop" << std::endl;
    service_.stop();
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

void StateMachine::taskComplete(boost::shared_ptr<Task> nextTask)
{
    std::cout << "[StateMachine] current task complete" << message_ << std::endl;
    if (nextTask)
    {
        service_.post(boost::bind(&Task::execute, nextTask));
    }
}
