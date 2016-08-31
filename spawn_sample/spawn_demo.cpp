/*
 * spawn_demo.cpp
 *
 *  Created on: Aug 29, 2016
 *  Author: rpartridge
 *
 *  This example demonstrates waiting for Boost::ASIO timer
 *  using a coroutine instead of an asynchronous callback.
 *
 *  When the call to async_wait is made passing in the
 *  boost::asio::yield_context, the call appears to block.
 *  Instead of actually blocking, the call stack is suspended
 *  and the IO service resumes, processing the timer. When
 *  the timer expires, the handler causes the suspended
 *  context to resume where the async_wait call was made.
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
