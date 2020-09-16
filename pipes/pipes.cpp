#include <iostream>
#include <sstream>
#include <memory>
#include <cstdint>
#include <future>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/ref.hpp>
#include <boost/process.hpp>
#include <boost/process/async_pipe.hpp>
#include <boost/process/extend.hpp>
#include <boost/process/io.hpp>
#include <boost/algorithm/string/trim.hpp>

class ProcessReader :
    public std::enable_shared_from_this<ProcessReader>
{
public:
    using ProcessExitCallback = std::function<void(int32_t, const std::error_code&)>;

    explicit ProcessReader(boost::asio::io_context& io_context) :
        io_context_(io_context)
    {}

    void runProcess()
    {
        statusPipe_ = std::unique_ptr<boost::process::async_pipe>(new boost::process::async_pipe(io_context_));
        const auto sinkFd = statusPipe_->native_sink();
        auto commandStream = std::ostringstream();
        commandStream << "/home/rpartridge/github/scratchpad/fd_writer/build/fd_writer " << sinkFd << " 100";

        auto callback = boost::bind(&ProcessReader::handleProcessComplete, shared_from_this(), boost::placeholders::_1, boost::placeholders::_2);
        asyncHandler_ = std::unique_ptr<AsyncProcessHandler>(new AsyncProcessHandler(callback));
        childProcess_ = std::unique_ptr<boost::process::child>(new boost::process::child(commandStream.str(), *asyncHandler_, boost::process::std_out > output_, boost::process::std_err > errorOutput_, io_context_));

        boost::asio::async_read_until(*statusPipe_, statusBuffer_, "\n", boost::bind(&ProcessReader::handleStatusMessage, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

private:
    class AsyncProcessHandler :
        public boost::process::extend::async_handler
    {
    public:
        explicit AsyncProcessHandler(ProcessExitCallback callback) :
            callback_(callback)
        {}

    template <typename Exec>
    std::function<void(int32_t, const std::error_code&)> on_exit_handler(Exec& /* exec */)
    {
        auto callback = this->callback_;
        return [callback](int32_t exitCode, const std::error_code& ec)
        {
            callback(exitCode, ec);
        };
    }

    private:
        ProcessExitCallback callback_;
    };

    void handleStatusMessage(const boost::system::error_code& ec, size_t bytesTransferred)
    {
        if (!ec)
        {
            if (bytesTransferred > 0)
            {
                auto buffers = statusBuffer_.data();
                auto data = std::string(static_cast<const char*>(buffers.data()), bytesTransferred);
                statusBuffer_.consume(bytesTransferred);
                boost::algorithm::trim(data);
                std::cout << "status message: " << data << std::endl;
            }
            boost::asio::async_read_until(*statusPipe_, statusBuffer_, "\n", boost::bind(&ProcessReader::handleStatusMessage, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
        else if (ec != boost::asio::error::eof &&
                    ec != boost::system::errc::operation_canceled &&
                    ec != boost::asio::error::operation_aborted)
        {
            std::cerr << "error reading status: " << ec.message() << std::endl;
        }
    }

    void handleProcessComplete(int32_t exitCode, const std::error_code& ec)
    {
        std::cout << "Process completed with exit code: " << exitCode << std::endl;
        if (statusPipe_)
        {
            statusPipe_->close();
            statusPipe_.reset();
        }
        if (output_.valid())
        {
            auto outputMessage = output_.get();
            boost::algorithm::trim(outputMessage);
            if (outputMessage.size() > 0)
            {
                std::cout << "output message: " << outputMessage << std::endl;
            }
        }
        if (errorOutput_.valid())
        {
            auto errMessage = errorOutput_.get();
            boost::algorithm::trim(errMessage);
            if (errMessage.size() > 0)
            {
                std::cout << "error message: " << errMessage << std::endl;
            }
        }
        else
        {
            std::cerr << "error getting the error output -- future not valid" << std::endl;
        }
    }

    boost::asio::io_context& io_context_;
    std::unique_ptr<boost::process::child> childProcess_;
    std::unique_ptr<boost::process::async_pipe> statusPipe_;
    boost::asio::streambuf statusBuffer_;
    std::unique_ptr<AsyncProcessHandler> asyncHandler_;
    std::future<std::string> output_;
    std::future<std::string> errorOutput_;
};

int main(int argc, char* argv[])
{
    std::cout << "Hello world!" << std::endl;
    auto pid = getpid();
    std::cout << "PID: " << pid << std::endl;

    boost::asio::io_context io_context;
    auto pr = std::make_shared<ProcessReader>(io_context);
    pr->runProcess();

    io_context.run();

    std::cout << "All done!" << std::endl;
}
