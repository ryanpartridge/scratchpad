/*
 * dlopen_timer_lib.cpp
 *
 *  Created on: Oct 10, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

using namespace std;

void libOnTimer(boost::shared_ptr<boost::asio::steady_timer> timer, const boost::system::error_code& ec)
{
    if (ec)
    {
        cout << "[libOnTimer] error: " << ec.message() << endl;
    }
    else
    {
        cout << "[libOnTimer] timer expired" << endl;
    }
    //timer->cancel();
}

extern "C"
void libStartTimer(boost::asio::io_service& io)
{
    cout << "[libStartTimer] address of io_service: " << &io << endl;
    cout << "[libStartTimer] setting timer" << endl;
    boost::shared_ptr<boost::asio::steady_timer> timer = boost::make_shared<boost::asio::steady_timer>(boost::ref(io));
    timer->expires_from_now(boost::chrono::seconds(10));
    timer->async_wait(boost::bind(&libOnTimer, timer, boost::asio::placeholders::error));
}
