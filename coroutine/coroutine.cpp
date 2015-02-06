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

void useTimer(boost::asio::io_service& service, boost::asio::yield_context yld)
{
    boost::system::error_code ec;
    boost::asio::steady_timer timer(service, boost::chrono::seconds(5));
    cout << "staring timer 1" << endl;
    timer.async_wait(yld[ec]);
    if (!ec)
    {
        cout << "timer 1 expired normally" << endl;
    }
    else
    {
        cout << "timer 1 expired with error: " << ec.message() << endl;
    }
    cout << "done with timer 1" << endl;
}

void useTimer2(boost::asio::io_service& service, boost::asio::yield_context yld)
{
    boost::system::error_code ec;
    boost::asio::steady_timer timer(service, boost::chrono::seconds(2));
    cout << "starting timer 2" << endl;
    timer.async_wait(yld[ec]);
    if (!ec)
    {
        cout << "timer 2 expired normally" << endl;
    }
    else
    {
        cout << "timer 2 expired with error: " << ec.message() << endl;
    }
    cout << "done with timer 2" << endl;
}

int main(int argc, char* argv[])
{
    cout << "entering main" << endl;

    boost::asio::io_service service;
    boost::asio::spawn(service, boost::bind(&useTimer, boost::ref(service), _1));
    boost::asio::spawn(service, boost::bind(&useTimer2, boost::ref(service), _1));
    boost::this_thread::sleep_for(boost::chrono::seconds(5));
    cout << "calling run()" << endl;
    service.run();

    cout << "exiting main" << endl;
    return 0;
}
