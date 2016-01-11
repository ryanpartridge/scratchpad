/*
 * async_future.cpp
 *
 *  Created on: Sep 8, 2015
 *      Author: rpartridge
 */

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_USES_MOVE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

int count = 0;

void printAnswer(boost::future<int> f)
{
    std::cout << "answer: " << f.get() << std::endl;
}

int doNotInterfere()
{
    std::cout << "doing some work" << std::endl;
    boost::this_thread::sleep_for(boost::chrono::seconds(5));
    return 42;
}

void startTask()
{
    std::cout << "starting task" << std::endl;

    //boost::async(doNotInterfere);
    //boost::packaged_task<int> task(doNotInterfere);
    //boost::future<int> f = task.get_future();
    boost::future<int> f1 = boost::async(boost::launch::async, doNotInterfere);
    boost::future<void> f2 = f1.then(boost::launch::async, boost::bind(&printAnswer, _1));
    f2.get();
    //boost::this_thread::sleep_for(boost::chrono::seconds(10));
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
    std::cout << "BOOST_THREAD_VERSION: " << BOOST_THREAD_VERSION << std::endl;
/*
#if defined BOOST_THREAD_PROVIDES_FUTURE
    std::cout << "BOOST_THREAD_PROVIDES_FUTURE defined" << std::endl;
#endif

#if defined BOOST_THREAD_USES_CHRONO
    std::cout << "BOOST_THREAD_USES_CHRONO defined" << std::endl;
#endif

#if defined BOOST_THREAD_PROVIDES_INTERRUPTIONS
    std::cout << "BOOST_THREAD_PROVIDES_INTERRUPTIONS defined" << std::endl;
#endif

#if defined BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
    std::cout << "BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY defined" << std::endl;
#endif

#if defined BOOST_THREAD_USES_MOVE
    std::cout << "BOOST_THREAD_USES_MOVE defined" << std::endl;
#endif
*/
#if defined BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
    std::cout << "BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION defined" << std::endl;
#endif

    boost::asio::io_service io_service;
    boost::asio::steady_timer timer(io_service, boost::chrono::seconds(1));
    timer.async_wait(boost::bind(&timedPrint, boost::ref(timer), boost::ref(io_service)));
    io_service.post(boost::bind(&startTask));
    std::cout << "starting io_service" << std::endl;
    io_service.run();
    std::cout << "io_service complete" << std::endl;

    try
    {
        boost::filesystem::path tempPath = boost::filesystem::unique_path();
        std::cout << "Temporary path: " << tempPath.string() << std::endl;
    }
    catch (const boost::filesystem::filesystem_error& fe)
    {
        std::cout << "Error generating temporary path: " << fe.what() << std::endl;
    }

    return 0;
}
