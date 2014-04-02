/*
 * timer_expire.cpp
 *
 *  Created on: Feb 19, 2014
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
#include <boost/phoenix.hpp>

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

int main(int argc, char* argv[])
{
    cout << "entering main" << endl;

    boost::asio::io_service io_service;
    boost::shared_ptr<boost::asio::steady_timer> timer = boost::make_shared<boost::asio::steady_timer>(boost::ref(io_service), boost::chrono::seconds(5));
    timer->async_wait(&handler);
    boost::shared_ptr<boost::thread> timer_thread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, boost::ref(io_service)));

    boost::this_thread::sleep_for(boost::chrono::seconds(2));
    if (timer->expires_from_now() > boost::chrono::seconds(1))
    {
        cout << "timer has not yet expired" << endl;
    }
    timer->cancel();
    boost::this_thread::sleep_for(boost::chrono::seconds(4));
    if (timer)
    {
        cout << "timer still valid" << endl;
        if (timer->expires_at() < boost::chrono::steady_clock::now())
        {
            cout << "timer has expired" << endl;
        }
    }
    else
    {
        cout << "timer not valid" << endl;
    }

    timer_thread->join();

    boost::function<void(int, int)> f = (cout << boost::phoenix::arg_names::arg1 << endl, cout << boost::phoenix::val("hello") << endl);
    f(1, 2);

    cout << "exiting main" << endl;
    return 0;
}
