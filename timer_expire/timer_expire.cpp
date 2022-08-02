#include <iostream>
#include <memory>
#include <functional>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

void handler(const boost::system::error_code& ec)
{
    std::cout << "entering handler" << std::endl;
    if (!ec)
    {
        std::cout << "timer expired" << std::endl;
    }
    else if (ec == boost::asio::error::operation_aborted)
    {
        std::cout << "timer canceled" << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::cout << "entering main" << std::endl;

    std::shared_ptr<boost::asio::io_context> io_context = std::make_shared<boost::asio::io_context>();
    std::shared_ptr<boost::asio::steady_timer> timer = std::make_shared<boost::asio::steady_timer>(*io_context, std::chrono::seconds(10));
    timer->async_wait(&handler);
    std::cout << "starting io_context thread" << std::endl;
    std::shared_ptr<boost::thread> timer_thread = std::make_shared<boost::thread>([io_context]() { io_context->run(); });

    boost::this_thread::sleep_for(boost::chrono::seconds(2));
    if (timer->expires_at() > std::chrono::steady_clock::now())
    {
        std::cout << "timer has not yet expired" << std::endl;
    }

    boost::this_thread::sleep_for(boost::chrono::seconds(4));
    if (timer)
    {
        std::cout << "timer still valid" << std::endl;
        if (timer->expires_at() < std::chrono::steady_clock::now())
        {
            std::cout << "timer has expired" << std::endl;
        }
    }
    else
    {
        std::cout << "timer not valid" << std::endl;
    }

    std::cout << "destroying timer" << std::endl;
    timer.reset();

    std::cout << "waiting for io_context thread" << std::endl;
    timer_thread->join();
    std::cout << "io_context thread complete" << std::endl;

    std::cout << "exiting main" << std::endl;
    return 0;
}
