#include <iostream>
#include <memory>
#include <functional>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

class Runner :
    public std::enable_shared_from_this<Runner>
{
public:
    explicit Runner(boost::asio::io_context& io_context) :
        io_context_(io_context)
    {}

    ~Runner() = default;

    void start(boost::asio::yield_context yield)
    {
        std::cout << "staring runner" << std::endl;
        timer_ = std::make_shared<boost::asio::steady_timer>(io_context_, std::chrono::seconds(2));

/*
        timer_->async_wait(
            [](boost::system::error_code const& ec)
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

        boost::system::error_code ec;
        timer_->async_wait(yield[ec]);
        if (ec)
        {
            std::cerr << "timer error: " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "timer expired" << std::endl;
        }

        std::cout << "leaving the start function" << std::endl;
    }

private:
    boost::asio::io_context& io_context_;
    std::shared_ptr<boost::asio::steady_timer> timer_;
};

int main(int argc, char* argv[])
{
    std::cout << "async_to_sync startup" << std::endl;
    boost::asio::io_context io_context;
    auto runner = std::make_shared<Runner>(io_context);
    boost::asio::spawn(std::bind(&Runner::start, runner, std::placeholders::_1));
    io_context.run();
    std::cout << "async_to_sync complete" << std::endl;
    return 0;
}
