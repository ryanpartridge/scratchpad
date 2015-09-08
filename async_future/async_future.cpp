/*
 * async_future.cpp
 *
 *  Created on: Sep 8, 2015
 *      Author: rpartridge
 */

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

int count = 0;

void startTask()
{
    std::cout << "starting task" << std::endl;
    boost::this_thread::sleep_for(boost::chrono::seconds(5));
    std::cout << "task complete" << std::endl;
}

void timedPrint(boost::asio::steady_timer& timer, boost::asio::io_service& io_service)
{
    std::cout << "ping (" << count << ")" << std::endl;
    timer.expires_from_now(boost::chrono::seconds(1));
    if (++count < 10)
    {
        timer.async_wait(boost::bind(&timedPrint, boost::ref(timer), boost::ref(io_service)));
    }
}

int main(int argc, char* argv[])
{
    boost::asio::io_service io_service;
    boost::asio::steady_timer timer(io_service, boost::chrono::seconds(1));
    timer.async_wait(boost::bind(&timedPrint, boost::ref(timer), boost::ref(io_service)));
    io_service.post(boost::bind(&startTask));
    std::cout << "starting io_service" << std::endl;
    io_service.run();
    std::cout << "io_service complete" << std::endl;
    return 0;
}
