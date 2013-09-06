/*
 * Server.hpp
 *
 *  Created on: Sep 5, 2013
 *      Author: rpartridge
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <boost/asio.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>

#include <StateMachine.hpp>

class Server :
    public boost::enable_shared_from_this<Server>
{
public:
    Server(boost::asio::io_service& io_service);
    virtual ~Server();

    void start();

private:
    boost::asio::io_service& mainService_;
    boost::asio::io_service workerService_;
    StateMachine machine_;
};

#endif /* SERVER_HPP_ */
