#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <functional>

#include <boost/asio.hpp>
#include <boost/coroutine2/all.hpp>

void coFib(boost::coroutines2::asymmetric_coroutine<uint32_t>::push_type& sink)
{
    std::cout << "entering coFib" << std::endl;
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
    std::cout << std::endl << "exiting coFib" << std::endl;
}

class Runner:
    public std::enable_shared_from_this<Runner>
{
public:
    using GetValueHandler = std::function<void(uint32_t)>;

    explicit Runner(boost::asio::io_context& io_context) :
        io_context_(io_context)
    {
    }

    ~Runner() = default;

    void start()
    {
        std::cout << "entering start()" << std::endl;

        auto instance = shared_from_this();
        workflowTimer_ = std::make_shared<boost::asio::steady_timer>(io_context_, std::chrono::seconds(2));
        workflowTimer_->async_wait(
            [instance](boost::system::error_code const& ec)
            {
                if (ec)
                {
                    std::cerr << "workflow timer error: " << ec.message() << std::endl;
                }
                else
                {
                    auto value = instance->getValue();
                    std::cout << "synchronous value: " << value << std::endl;

                    instance->workflowTimer_->expires_after(std::chrono::seconds(2));
                    instance->workflowTimer_->async_wait(
                        [instance](boost::system::error_code const& otherEc)
                        {
                            if (otherEc)
                            {
                                std::cerr << "workflow timer error: " << otherEc.message() << std::endl;
                            }
                            else
                            {
                                instance->asyncGetValue(
                                    [instance](uint32_t asyncValue)
                                    {
                                        std::cout << "asynchronous value: " << asyncValue << std::endl;

                                        instance->workflowTimer_->expires_after(std::chrono::seconds(2));
                                        instance->workflowTimer_->async_wait(
                                            [instance](boost::system::error_code const& finalEc)
                                            {
                                                if (finalEc)
                                                {
                                                    std::cerr << "workflow timer error: " << finalEc.message() << std::endl;
                                                }
                                                else
                                                {
                                                    auto wrappedValue = instance->getValueWrapped();
                                                    std::cout << "wrapped value: " << wrappedValue << std::endl;
                                                }
                                            }
                                        );
                                    }
                                );
                            }
                        }
                    );
                }
            }
        );

        std::cout << "leaving start()" << std::endl;
    }

    uint32_t getValue()
    {
        std::cout << "entering getValue()" << std::endl;

        // synchronous call
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "leaving getValue()" << std::endl;
        return 1;
    }

    void asyncGetValue(GetValueHandler handler)
    {
        std::cout << "entering asyncGetValue()" << std::endl;

        // asynchronous call
        auto instance = shared_from_this();
        getValueTimer_ = std::make_shared<boost::asio::steady_timer>(io_context_, std::chrono::seconds(2));
        getValueTimer_->async_wait(
            [instance, handler](boost::system::error_code const& ec)
            {
                handler(2);
            }
        );

        std::cout << "leaving asyncGetValue()" << std::endl;
    }

    uint32_t getValueWrapped()
    {
        std::cout << "entering getValueWrapped()" << std::endl;

        boost::coroutines2::asymmetric_coroutine<uint32_t>::pull_type source(
            std::bind(&Runner::getValueWrappedImpl, shared_from_this(), std::placeholders::_1));

        std::cout << "leaving getValueWrapped()" << std::endl;
        if (source)
        {
            return source.get();
        }
        else
        {
            std::cerr << "problem getting the wrapped value" << std::endl;
            return UINT32_MAX;
        }
    }

private:
    void getValueWrappedImpl(boost::coroutines2::asymmetric_coroutine<uint32_t>::push_type& sink)
    {
        auto instance = shared_from_this();
        instance->asyncGetValue(
            [instance, &sink](uint32_t value)
            {
                std::cout << "wrapped asyncGetValue finished -- pushing value: " << value << std::endl;
                //sink(value);
            }
        );
    }

    boost::asio::io_context& io_context_;
    std::shared_ptr<boost::asio::steady_timer> workflowTimer_;
    std::shared_ptr<boost::asio::steady_timer> getValueTimer_;
};

int main(int argc, char* argv[])
{
    std::cout << "starting coro2" << std::endl;
    std::cout << std::endl;

/*
    // create (and pass execution to) coroutine
    // loop over the object to get Fibnacci numbers
    std::cout << "instantiating pull_type with coFib" << std::endl;
    boost::coroutines2::asymmetric_coroutine<uint32_t>::pull_type source(&coFib);
    std::cout << "starting loop" << std::endl;
    for(auto i: source)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl << "loop is complete" << std::endl;
*/

    boost::asio::io_context io_context;
    auto runner = std::make_shared<Runner>(io_context);

/*
    auto value = runner->getValue();
    std::cout << "synchronous value: " << value << std::endl;

    std::cout << std::endl;

    runner->asyncGetValue(
        [](uint32_t value)
        {
            std::cout << "asynchronous value: " << value << std::endl;
        }
    );
*/

    runner->start();

    io_context.run();

    std::cout << std::endl;
    std::cout << "exiting coro2" << std::endl;
    return 0;
}
