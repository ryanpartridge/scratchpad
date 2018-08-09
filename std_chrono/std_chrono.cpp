/*
 * std_chrono.cpp
 *
 *  Created on: Aug 2, 2018
 *      Author: rpartridge
 */

#include <iostream>

//#define BOOST_ASIO_DISABLE_STD_CHRONO
#include <boost/asio.hpp>

#ifdef BOOST_ASIO_DISABLE_STD_CHRONO
#include <boost/chrono.hpp>
#else
#include <chrono>
#endif

void timer_callback(const boost::system::error_code& /*ec*/)
{
    std::cout << "timer callback invoked" << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "entering main" << std::endl;
    boost::asio::io_context io_context;
    boost::asio::steady_timer timer(io_context);
#ifdef BOOST_ASIO_DISABLE_STD_CHRONO
    std::cout << "std::chrono is disabled" << std::endl;
    timer.expires_after(boost::chrono::seconds(2));
#else
    std::cout << "std::chrono is enabled" << std::endl;
    timer.expires_after(std::chrono::seconds(2));
#endif
    std::cout << "starting timer" << std::endl;
    timer.async_wait(&timer_callback);
    io_context.run();
    std::cout << "exiting main" << std::endl;
    return 0;
}
