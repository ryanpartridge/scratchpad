/*
 * coroutine.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <boost/asio/spawn.hpp>

using namespace std;

void handler(const boost::system::error_code& ec)
{
    if (!ec)
    {
        cout << "timer expired" << endl;
    }
    else if (ec == boost::asio::error::operation_aborted)
    {
        cout << "timer canceled" << endl;
    }
}

void useTimer(boost::asio::io_service& service, boost::asio::yield_context yield)
{
    boost::system::error_code ec;
    boost::asio::steady_timer timer(service, boost::chrono::seconds(3));
    cout << "staring timer" << endl;
    timer.async_wait(yield[ec]);
    if (!ec)
    {
        cout << "timer expired normally" << endl;
    }
    else
    {
        cout << "timer expired with error: " << ec.message() << endl;
    }
    cout << "done with timer" << endl;
}

void useTimer2(boost::asio::io_service& service, int num)
{
    boost::asio::steady_timer timer(service, boost::chrono::seconds(num));
}

int main(int argc, char* argv[])
{
    cout << "entering main" << endl;

    boost::asio::io_service service;
    boost::asio::spawn(service, boost::bind(&useTimer, boost::ref(service), _1));
    service.run();

    cout << "exiting main" << endl;
    return 0;
}
