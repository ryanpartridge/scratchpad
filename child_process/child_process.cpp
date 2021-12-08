#include <iostream>
#include <memory>
#include <functional>
#include <cstdint>
#include <system_error>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <boost/process/extend.hpp>
#include <boost/process/async_pipe.hpp>

using ProcessExitCallback = std::function<void(int32_t, std::error_code const&)>;

class AptAsyncHandler :
    public boost::process::extend::async_handler
{
public:
    AptAsyncHandler() = default;

    AptAsyncHandler(AptAsyncHandler const&) = delete;

    explicit AptAsyncHandler(ProcessExitCallback callback) :
        callback_(callback)
    {}

    template<typename Exec>
    ProcessExitCallback on_exit_handler(Exec& /* exec */)
    {
        auto callback = this->callback_;
        return [callback](int32_t exitCode, std::error_code const& ec)
        {
            callback(exitCode, ec);
        };
    }

    ~AptAsyncHandler() = default;

private:
    ProcessExitCallback callback_;
};

class ProcessRunner :
    public std::enable_shared_from_this<ProcessRunner>
{
public:
    ProcessRunner() = default;
    ProcessRunner(ProcessRunner const&) = delete;

    ~ProcessRunner()
    {
        if (processTimer_)
        {
            processTimer_->cancel();
            processTimer_.reset();
        }
    }

    void start()
    {
        io_context_.run();
    }

    void asyncRunProcess()
    {
        std::cout << "running command: \"" << cmd << "\"" << std::endl;
        ProcessExitCallback callback = std::bind(&ProcessRunner::exitCallback, shared_from_this(), std::placeholders::_1, std::placeholders::_2);
        asyncHandler_ = std::make_unique<AptAsyncHandler>(callback);
        childProcess_ = std::make_unique<boost::process::child>(cmd, *asyncHandler_, boost::process::std_out > boost::process::null, boost::process::std_err > boost::process::null, io_context_);
        timestamp_ = std::chrono::steady_clock::now();

        auto instance = shared_from_this();
        processTimer_ = std::make_unique<boost::asio::steady_timer>(io_context_, std::chrono::seconds(120));
        std::cout << "starting timer" << std::endl;
        processTimer_->async_wait(
            [instance](boost::system::error_code const& timer_ec)
            {
                if (timer_ec)
                {
                    std::cout << "timer error (" << timer_ec.value() << "): " << timer_ec.message() << std::endl;
                }
                else
                {
                    std::cout << "timer expired";
                    auto elapsed = std::chrono::steady_clock::now() - instance->timestamp_;
                    std::cout << " (" << std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() << " seconds elapsed)";
                    if (elapsed > std::chrono::minutes(2))
                    {
                        if (instance->childProcess_)
                        {
                            std::cout << " -- killing child process";
                            std::error_code processEc;
                            instance->childProcess_->terminate(processEc);
                        }
                    }
                    else
                    {
                        auto remaining = std::chrono::minutes(2) - elapsed;
                        std::cout << " -- not enough time passed (" << std::chrono::duration_cast<std::chrono::seconds>(remaining + std::chrono::seconds(5)).count() << " remaining)";
                    }
                    std::cout << std::endl;
                }
                instance->processTimer_.reset();
            }
        );
    }

    void exitCallback(int32_t exitCode, std::error_code const& ec)
    {
        if (processTimer_)
        {
            processTimer_->cancel();
            processTimer_.reset();
        }
        childProcess_.reset();
        std::cout << "process exit code: " << exitCode << std::endl;
        if (ec)
        {
            std::cout << "process error (" << ec.value() << "): " << ec.message() << std::endl;
        }
    }

private:
    const std::string cmd{"/usr/bin/sleep 125"};
    boost::asio::io_context io_context_;
    std::chrono::steady_clock::time_point timestamp_;
    std::unique_ptr<boost::process::child> childProcess_;
    std::unique_ptr<AptAsyncHandler> asyncHandler_;
    std::unique_ptr<boost::asio::steady_timer> processTimer_;
};

int main(int argc, char* argv[])
{
    std::cout << "child_process program" << std::endl;
    auto runner = std::make_shared<ProcessRunner>();
    runner->asyncRunProcess();
    runner->start();
    return 0;
}