/*
 * StateMachine.hpp
 *
 *  Created on: Sep 5, 2013
 *      Author: ryan
 */

#ifndef STATEMACHINE_HPP_
#define STATEMACHINE_HPP_

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

class Server;

class StateMachine
{
public:
    StateMachine(boost::asio::io_service& io_service);
    virtual ~StateMachine();

    void initialize(boost::shared_ptr<Server> server);
    void start();
    void pushMessage(const std::string& message);
    const std::string popMessage();

private:
    boost::asio::io_service& service_;
    boost::shared_ptr<Server> server_;
    mutable boost::mutex mutex_;
    boost::condition_variable  messageWaiting_;
    std::string message_;
};

#endif /* STATEMACHINE_HPP_ */
