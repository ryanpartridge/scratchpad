/*
 * StateMachine.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: ryan
 */

#include <boost/smart_ptr.hpp>

#include <StateMachine.hpp>

StateMachine::StateMachine(boost::asio::io_service& io_service) :
    service_(io_service)
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::initialize(boost::shared_ptr<Server> server)
{
    server_ = server;
}

void StateMachine::pushMessage(const std::string& message)
{
    boost::mutex::scoped_lock lock(mutex_);
    message_ = message;
    messageWaiting_.notify_all();
}

const std::string StateMachine::popMessage()
{
    boost::mutex::scoped_lock lock(mutex_);
    while (message_.empty())
    {
        messageWaiting_.wait(lock);
    }
    return message_;
}

void StateMachine::start()
{

}
