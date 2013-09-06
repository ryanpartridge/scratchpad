/*
 * state_machine.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: rpartridge
 */

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

#include <Server.hpp>

int main(int argc, char* argv[])
{
    boost::asio::io_service io_service;
    boost::shared_ptr<Server> server = boost::make_shared<Server>(boost::ref(io_service));
    server->start();
    io_service.run();
    return 0;
}
