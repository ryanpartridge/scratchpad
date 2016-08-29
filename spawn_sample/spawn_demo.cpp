/*
 * spawn_demo.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>

void useTimer(boost::asio::io_service& io_service, boost::asio::yield_context yield)
{
    std::cout << "entering useTimer" << std::endl;
    boost::system::error_code ec;
    boost::asio::steady_timer timer(io_service, boost::chrono::seconds(5));
    std::cout << "calling async_wait" << std::endl;
    timer.async_wait(yield[ec]);
    std::cout << "timer finished" << std::endl;

    if (ec)
    {
        std::cout << "timer error:" << ec.message() << std::endl;
    }

    std::cout << "exiting useTimer" << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "entering main" << std::endl;
    boost::asio::io_service io_service;

    boost::asio::spawn(io_service, boost::bind(&useTimer, boost::ref(io_service), _1));

    std::cout << "starting the io_service" << std::endl;
    io_service.run();
    std::cout << "io_service is complete" << std::endl;
    std::cout << "exiting main" << std::endl;
    return 0;
}
