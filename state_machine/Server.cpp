/*
 * Server.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: rpartridge
 */

#include <Server.hpp>

Server::Server(boost::asio::io_service& io_service) :
    mainService_(io_service),
    machine_(workerService_)
{

}

Server::~Server()
{

}

void Server::start()
{

    machine_.initialize(this->shared_from_this());
    machine_.start();
}
