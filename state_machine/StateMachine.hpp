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
#include <boost/weak_ptr.hpp>
#include <string>

#include <State.hpp>

class Server;

class StateMachine
{
public:
    StateMachine(boost::asio::io_service& io_service, boost::shared_ptr<Server> server);
    virtual ~StateMachine();

    void start();
    void stop();
    boost::asio::io_service& io_service();
    void pushMessage(const std::string& message);
    const std::string popMessage();
    void handleStateComplete(boost::shared_ptr<State> nextState);

private:
    boost::asio::io_service& service_;
    //boost::asio::io_service::work work_;
    boost::weak_ptr<Server> server_;
    mutable boost::mutex mutex_;
    boost::condition_variable  messageWaiting_;
    std::string message_;
    bool running_;
};

#endif /* STATEMACHINE_HPP_ */
