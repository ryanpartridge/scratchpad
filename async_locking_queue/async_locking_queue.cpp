/*
 * async_locking_queue.cpp
 *
 *  Created on: Dec 13, 2013
 *      Author: ryan
 */

#include <string>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/chrono.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include <LockingQueue.hpp>

boost::shared_ptr<boost::thread> waitThread;

void produceStrings(StringQueue& queue)
{
    char c = 'a';
    while (true)
    {
        std::string str;
        str += c;
        queue.push(str);
        if (++c > 'z')
        {
            c = 'a';
        }
        boost::this_thread::sleep_for(boost::chrono::seconds(5));
    }
}

void consumeStrings(StringQueue& queue, boost::asio::io_service& io_service)
{
    waitThread = boost::make_shared<boost::thread>(boost::bind(&waitForQueueValue, boost::ref(queue), boost::ref(io_service)));
}

void handleQueueValue(const std::string& value, StringQueue& queue, boost::asio::io_service& io_service)
{
    std::cout << "Value: " << value << std::endl;
}

void handleTimerExpired(const boost::system::error_code& ec)
{
    std::cout << "timer expired" << std::endl;
}

void waitForQueueValue(StringQueue& queue, boost::asio::io_service& io_service)
{
    std::string value;
    queue.pop(value);
    io_service.post(boost::bind(&handleQueueValue, value, boost::ref(queue), boost::ref(io_service)));
}

int main(int argc, char* argv[])
{
    StringQueue queue;
    boost::asio::io_service io_service;
    boost::asio::io_service::work work(io_service);
    boost::asio::steady_timer timer(io_service, boost::chrono::seconds(2));

    io_service.post(boost::bind(&consumeStrings, boost::ref(queue), boost::ref(io_service)));
    timer.async_wait(&handleTimerExpired);

    boost::thread pThread(boost::bind(&produceStrings, boost::ref(queue)));
    boost::thread cThread(boost::bind(&boost::asio::io_service::run, boost::ref(io_service)));

    pThread.join();
    cThread.join();
    return 0;
}
