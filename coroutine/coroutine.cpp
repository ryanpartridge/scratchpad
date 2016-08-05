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
#include <boost/coroutine/all.hpp>
#include <boost/foreach.hpp>

using namespace std;

void coHandler()
{
    cout << "invoking handler" << endl;
}

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

void useTimer2(boost::asio::io_service& service, boost::asio::yield_context yld)
{
    boost::system::error_code ec;
    boost::asio::steady_timer timer(service, boost::chrono::seconds(3));
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

void useTimer3(boost::asio::io_service& service, boost::asio::yield_context yield)
{
    boost::system::error_code ec;
    boost::asio::steady_timer timer(service, boost::chrono::seconds(5));
    cout << "starting timer 3" << endl;
    timer.async_wait(yield[ec]);
    if (!ec)
    {
        cout << "timer 3 expired normally" << endl;
    }
    else
    {
        cout << "timer 3 expired with error: " << ec.message() << endl;
    }
    cout << "done with timer 3" << endl;
}

//typedef boost::function<void(boost::coroutines::asymmetric_coroutine<int>::push_type&)> fibCoFunc;
void coFib(boost::coroutines::asymmetric_coroutine<int>::push_type& sink)
{
    cout << "entering coFib" << endl;
    int first = 1, second = 1;
    sink(first);
    sink(second);

    for (int i = 0; i < 8; ++i)
    {
        int third = first + second;
        first = second;
        second = third;
        sink(third);
    }
    cout << endl << "exiting coFib" << endl;
}

typedef boost::function<void(boost::coroutines::symmetric_coroutine<int>::yield_type& yield)> symCoFunc;

void symCoC(boost::coroutines::symmetric_coroutine<int>::yield_type& yield)
{
    int i = yield.get();
    cout << "symCoC input: " << i << endl;
    yield();
    cout << "leaving symCoC" << endl;
}

void symCoB(boost::coroutines::symmetric_coroutine<int>::yield_type& yield)
{
    int i = yield.get();
    cout << "symCoB input: " << i << endl;
    boost::coroutines::symmetric_coroutine<int>::call_type func(&symCoC);
    func(3);
    cout << "leaving symCoB" << endl;
}

void symCoA(boost::coroutines::symmetric_coroutine<int>::yield_type& yield)
{
    int i = yield.get();
    cout << "symCoA input: " << i << endl;
    boost::coroutines::symmetric_coroutine<int>::call_type func(&symCoB);
    func(2);
    cout << "leaving symCoA" << endl;
}

void coSink(boost::coroutines::asymmetric_coroutine<int>::pull_type& sink)
{
    cout << "coSink invokes sink" << endl;
    sink();
    cout << "this line should never be executed!!!" << endl;
}

void coReturn(boost::coroutines::asymmetric_coroutine<int>::pull_type& sink)
{
    cout << "coReturn does nothing but return -- it does not invoke sink" << endl;
    return;
}

class MyHandler
{
public:
    void coHandler()
    {
        cout << "invoking MyHandler::coHandler" << endl;
    }
};

int main(int argc, char* argv[])
{
    cout << "entering main" << endl;

    typedef boost::function<void()> CoHandlerType;
    //typedef boost::function<void(boost::asio::yield_context)> CoFunc;
    CoHandlerType handlerFunc(&coHandler);

    MyHandler myHandler;

    boost::asio::io_service service;
    boost::asio::io_service::strand strand(service);
    strand.wrap(&coHandler);
    strand.wrap(boost::bind(&MyHandler::coHandler, myHandler));
    boost::asio::spawn(service, boost::bind(&useTimer, boost::ref(service), _1));
//    boost::asio::spawn(service, boost::bind(&useTimer2, boost::ref(service), _1));
//    boost::asio::spawn(strand.wrap(&coHandler), boost::bind(&useTimer3, boost::ref(service), _1));
    cout << "calling run()" << endl;
    service.run();
    cout << "run() has finished" << endl;
/*
    cout << "instantiating pull_type with coFib" << endl;
    boost::coroutines::asymmetric_coroutine<int>::pull_type source(&coFib);

    cout << "starting loop" << endl;
    BOOST_FOREACH (const int i, source)
    {
        cout << i << " ";
    }
    cout << "loop is complete" << endl;

    boost::coroutines::symmetric_coroutine<int>::call_type func(&symCoA);
    func(1);

    cout << "instantiating push_type with coSink" << endl;
    boost::coroutines::asymmetric_coroutine<int>::push_type sourceSink(&coSink);
    sourceSink(1);

    cout << "instantiating push_type with coReturn" << endl;
    boost::coroutines::asymmetric_coroutine<int>::push_type sourceReturn(&coReturn);
    sourceReturn(1);
*/
    cout << "exiting main" << endl;
    return 0;
}
