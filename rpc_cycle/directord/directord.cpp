/*
 * directord.cpp
 *
 *  Created on: Feb 13, 2015
 *      Author: rpartridge
 */

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/spawn.hpp>

void startTimer(boost::asio::io_service& io_service, boost::asio::yield_context yield_ctx)
{
    boost::shared_ptr<boost::asio::steady_timer> timer =
        boost::make_shared<boost::asio::steady_timer>(boost::ref(io_service), boost::chrono::seconds(5));
    boost::system::error_code ec;
    std::cout << "starting timer" << std::endl;
    timer->async_wait(yield_ctx[ec]);
    std::cout << "timer done" << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "starting up" << std::endl;
    boost::asio::io_service io_service;
    boost::asio::spawn(io_service, boost::bind(&startTimer, boost::ref(io_service), _1));
    io_service.run();
    std::cout << "shutting down" << std::endl;
}
