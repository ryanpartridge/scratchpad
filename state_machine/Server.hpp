/*
 * Server.hpp
 *
 *  Created on: Sep 5, 2013
 *      Author: rpartridge
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <boost/asio.hpp>

class Server
{
public:
    Server(boost::asio::io_service& io_service);
    virtual ~Server();

private:
    boost::asio::io_service& io_service_;
};


#endif /* SERVER_HPP_ */
