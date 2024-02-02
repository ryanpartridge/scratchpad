#include <iostream>
#include <chrono>
#include <memory>
#include <functional>
#include <type_traits>

#include <boost/thread/future.hpp>
#include <boost/asio.hpp>


template <class F>
typename std::function<F>::result_type launchTask(std::function<F> func)
{
    return func();
}

template <class F>
boost::future<typename std::function<F>::result_type> postTask(boost::asio::io_context& io_context, std::function<F> func)
{
    using return_type = typename std::function<F>::result_type;
    auto task = boost::packaged_task<return_type>(func);
    auto future = task.get_future();
    boost::asio::post(io_context, std::move(task));
    return future;
}

int meaninglessTask()
{
    std::cout << "this is a meaningless task" << std::endl;
    return 42;
}

void eventLoop(boost::asio::io_context& io_context)
{
    std::cout << "starting event loop" << std::endl;
    auto task = boost::packaged_task<int>(&meaninglessTask);
    auto future = task.get_future().share();
    boost::asio::post(io_context, std::move(task));

    auto timer = std::make_shared<boost::asio::steady_timer>(io_context, std::chrono::seconds(5));
    timer->async_wait([&io_context, timer, future](boost::system::error_code const& ec)
        {
            if (ec)
            {
                std::cerr << "error in timer: " << ec.message() << std::endl;
            }
            else
            {
                std::cout << "timer expired normally" << std::endl;
                if (future.is_ready())
                {
                    if (future.has_value())
                    {
                        std::cout << "result: " << future.get() << std::endl;
                    }
                }
                else
                {
                    std::cout << "future not ready by expiration" << std::endl;
                }
                
            }
        }
    );

    return;
}

int myFunc(int a, bool b)
{
    std::cout << "int value: " << a << std::endl;
    std::cout << "bool value: " << std::boolalpha << b << std::endl;
    return 43;
}

int main(int argc, char* argv[])
{
    std::cout << "starting future_work" << std::endl;
    std::cout << "boost thread version: " << BOOST_THREAD_VERSION << std::endl;

    std::function<int()> func(std::bind(&myFunc, 1, false));
    auto value = launchTask(func);
    std::cout << "launch task return value: " << value << std::endl;

    auto anotherFunc = std::bind(&myFunc, 2, true);

    

    boost::asio::io_context io_context;
    auto work = boost::asio::make_work_guard(io_context);
    boost::asio::post(io_context, std::bind(&eventLoop, std::ref(io_context)));
    auto future = postTask(io_context, std::function<int()>(&meaninglessTask));
    auto future2 = postTask(io_context, std::function<int()>([]() -> int { std::cout << "lambda task" << std::endl; return 44;}));
    work.reset();
    io_context.run();
    std::cout << "post task return value: " << future.get() << std::endl;
    std::cout << "post task lambda return value: " << future2.get() << std::endl;

    std::cout << "exiting future_work" << std::endl;
}
