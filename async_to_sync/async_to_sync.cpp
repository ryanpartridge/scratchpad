#include <iostream>
#include <memory>
#include <functional>
#include <chrono>

#include <boost/asio.hpp>

void handleTimer(boost::system::error_code const& ec, std::shared_ptr<boost::asio::steady_timer> timer)
{
    if (ec)
    {
        std::cerr << "timer error: " << ec.message() << std::endl;
    }
    else
    {
        std::cout << "timer expired" << std::endl;
    }
    timer.reset();
}

void eventLoop(boost::asio::io_context& io_context)
{
    std::cout << "event loop starting" << std::endl;

    //auto timer = std::make_shared<boost::asio::steady_timer>(io_context, std::chrono::seconds(2));
/*
    timer->async_wait(
        [timer](boost::system::error_code const& ec)
        {
            if (ec)
            {
                std::cerr << "timer error: " << ec.message() << std::endl;
            }
            else
            {
                std::cout << "timer expired" << std::endl;
            }
        }
    );
*/

    std::cout << "event loop finished" << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "async_to_sync startup" << std::endl;
    boost::asio::io_context io_context;
    boost::asio::post(std::bind(&eventLoop, std::ref(io_context)));
    io_context.run();
    std::cout << "async_to_sync complete" << std::endl;
    return 0;
}
