#include <iostream>
#include <memory>
#include <functional>
#include <cstdint>
#include <system_error>
#include <chrono>
#include <exception>

#include <boost/asio.hpp>
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull"
#endif
#include <boost/process.hpp>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
#include <boost/process/extend.hpp>
#include <boost/process/async_pipe.hpp>

using ProcessExitCallback = std::function<void(int32_t, std::error_code const&)>;

class AsyncProcessHandler :
    public boost::process::extend::async_handler
{
public:
    AsyncProcessHandler() = default;

    AsyncProcessHandler(AsyncProcessHandler const&) = delete;

    explicit AsyncProcessHandler(ProcessExitCallback callback) :
        callback_(callback)
    {}

    template<typename Exec>
    ProcessExitCallback on_exit_handler(Exec& /* exec */)
    {
        std::cout << "grabbing the exit handler" << std::endl;
        auto callback = this->callback_;
        return [callback](int32_t exitCode, std::error_code const& ec)
        {
            callback(exitCode, ec);
        };
    }

    virtual ~AsyncProcessHandler()
    {
        std::cout << "AsyncProcessHandler destructor" << std::endl;
    }

private:
    ProcessExitCallback callback_;
};

class ProcessRunner :
    public std::enable_shared_from_this<ProcessRunner>
{
public:
    ProcessRunner() = delete;
    ProcessRunner(ProcessRunner const&) = delete;

    explicit ProcessRunner(boost::asio::io_context& io_context) :
        io_context_{io_context}
    {}

    virtual ~ProcessRunner()
    {
        std::cout << "ProcessRunner destructor" << std::endl;
    }

    auto asyncRunProcess() -> void
    {
        std::cout << "running command: \"" << cmd << "\"" << std::endl;
        auto processTimer = std::make_shared<boost::asio::steady_timer>(io_context_, std::chrono::seconds(13));
        auto callback = ProcessExitCallback{[self = shared_from_this(), processTimer](int32_t exitCode, std::error_code const& ec)
            {
                self->processComplete(exitCode, ec, processTimer);
            }
        };

        std::error_code startError;
        auto childProcess = std::make_shared<boost::process::child>(cmd, AsyncProcessHandler{callback}, boost::process::error(startError), boost::process::std_out > boost::process::null, boost::process::std_err > boost::process::null, io_context_);
        if (startError)
        {
            std::cout << "error creating process: " << startError.message() << std::endl;
            std::cout << "raw error: " << startError << std::endl;
            if (startError == std::errc::no_such_file_or_directory)
            {
                std::cout << "command does not exist" << std::endl;
            }
            return;
        }

        std::cout << "starting timer" << std::endl;
        auto timestamp = std::chrono::steady_clock::now();
        processTimer->async_wait(
            [self = shared_from_this(), timestamp, childProcess, callback](boost::system::error_code const& timer_ec)
            {
                self->processTimerComplete(timer_ec, timestamp, childProcess, callback);
            }
        );
    }

    auto processComplete(int32_t exitCode, std::error_code const& ec, std::shared_ptr<boost::asio::steady_timer> processTimer) -> void
    {
        if (processTimer)
        {
            processTimer->cancel();
        }
        std::cout << "process exit code: " << exitCode << std::endl;
        if (ec)
        {
            std::cout << "process error (" << ec.value() << "): " << ec.message() << std::endl;
        }
    }

    auto processTimerComplete(boost::system::error_code const& ec, std::chrono::steady_clock::time_point timestamp, std::shared_ptr<boost::process::child> childProcess, ProcessExitCallback exitCallback) -> void
    {
        if (ec)
        {
            if (ec == boost::asio::error::operation_aborted)
            {
                std::cout << "process timer canceled" << std::endl;
            }
            else
            {
                std::cerr << "process timer error (" << ec.value() << "): " << ec.message() << std::endl;
            }
        }
        else
        {
            std::cout << "process timer expired" << std::endl;
            auto elapsed = std::chrono::steady_clock::now() - timestamp;
            std::cout << "time elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << " milliseconds" << std::endl;
            if (childProcess && childProcess->valid())
            {
                std::error_code processEc;
                if (childProcess->running(processEc))
                {
                    std::cout << "killing child process" << std::endl;
                    processEc = {};
                    childProcess->terminate(processEc);
                }
                else
                {
                    std::cerr << "timer expired but the process is not runnning???" << std::endl;
                    std::cout << "invoking process callback directly" << std::endl;
                    boost::asio::post(io_context_,
                        [self = shared_from_this(), exitCallback]()
                        {
                            if (exitCallback)
                            {
                                exitCallback(0, {});
                            }
                        }
                    );
                }
            }
        }
    }

private:
    const std::string cmd{"/usr/bin/foo 5"};
    boost::asio::io_context& io_context_;
};

int main(int argc, char* argv[])
{
    std::cout << "child_process program (pid: " << getpid() << ")" << std::endl;
    boost::asio::io_context io_context;
    auto runner = std::make_shared<ProcessRunner>(io_context);
    boost::asio::post(io_context, [runner]() { runner->asyncRunProcess(); });
    std::cout << "staring the io_context" << std::endl;
    io_context.run();
    std::cout << "io_context complete" << std::endl;
    return 0;
}