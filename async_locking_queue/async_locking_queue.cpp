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
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/make_shared.hpp>
#include <boost/chrono.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include <LockingQueue.hpp>

class Consumer :
    public boost::enable_shared_from_this<Consumer>,
    private boost::noncopyable
{
public:
    Consumer(boost::asio::io_service& io_service, StringQueue& queue) :
        io_service_(io_service),
        queue_(queue),
        timerTimeout_(1)
    {
    }

    void consumeStrings()
    {
        timer_ = boost::make_shared<boost::asio::steady_timer>(boost::ref(io_service_), boost::chrono::seconds(timerTimeout_));
        timer_->async_wait(boost::bind(&Consumer::handleTimer, shared_from_this(), boost::asio::placeholders::error));
        io_service_.post(boost::bind(&Consumer::consumeOneString, shared_from_this()));
    }

private:
    void consumeOneString()
    {
        waitThread_ = boost::make_shared<boost::thread>(boost::bind(&Consumer::waitForQueueValue, shared_from_this()));
    }

    void waitForQueueValue()
    {
        std::string value;
        queue_.pop(value);
        io_service_.post(boost::bind(&Consumer::handleQueueValue, shared_from_this(), value));
    }

    void handleQueueValue(const std::string& value)
    {
        std::cout << "queue value: " << value << std::endl;
        consumeOneString();
    }

    void handleTimer(const boost::system::error_code& ec)
    {
        std::cout << "timer expired" << std::endl;
        timer_->expires_from_now(boost::chrono::seconds(timerTimeout_));
        timer_->async_wait(boost::bind(&Consumer::handleTimer, shared_from_this(), boost::asio::placeholders::error));
    }

    boost::asio::io_service& io_service_;
    StringQueue& queue_;
    boost::shared_ptr<boost::thread> waitThread_;
    boost::shared_ptr<boost::asio::steady_timer> timer_;
    int timerTimeout_;
};


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

int main(int argc, char* argv[])
{
    StringQueue queue;
    boost::asio::io_service io_service;
    boost::asio::io_service::work work(io_service);

    boost::shared_ptr<Consumer> consumer = boost::make_shared<Consumer>(boost::ref(io_service), boost::ref(queue));
    consumer->consumeStrings();

    boost::thread pThread(boost::bind(&produceStrings, boost::ref(queue)));
    boost::thread cThread(boost::bind(&boost::asio::io_service::run, boost::ref(io_service)));

    pThread.join();
    cThread.join();
    return 0;
}
