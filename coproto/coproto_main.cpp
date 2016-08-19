/*
 * coproto.cpp
 *
 *  Created on: Aug 9, 2016
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
#include <boost/coroutine/all.hpp>
#include <boost/foreach.hpp>

using namespace std;

void useTimer(boost::asio::io_service& service, boost::asio::yield_context yld)
{
    boost::system::error_code ec;
    boost::asio::steady_timer timer(service, boost::chrono::seconds(6));
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

int main(int argc, char* argv[])
{
    cout << "entering main" << endl;

    boost::asio::io_service service;
    boost::asio::spawn(service, boost::bind(&useTimer, boost::ref(service), _1));
    cout << "calling run()" << endl;
    service.run();
    cout << "run() has finished" << endl;
    cout << "exiting main" << endl;
    return 0;
}
