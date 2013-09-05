/*
 * Service.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: rpartridge
 */

#include <Server.hpp>

Server::Server(boost::asio::io_service& io_service) :
    io_service_(io_service)
{

}

Server::~Server()
{

}
