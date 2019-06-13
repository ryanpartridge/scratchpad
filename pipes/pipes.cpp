#include <iostream>
#include <sstream>
#include <memory>
#include <fcntl.h>

#include <boost/cstdint.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/process.hpp>
#include <boost/process/async_pipe.hpp>
#include <boost/process/extend.hpp>
#include <boost/process/io.hpp>
#include <boost/algorithm/string/trim.hpp>

class AsyncHandler : public boost::process::extend::async_handler
{
public:
    AsyncHandler(std::shared_ptr<boost::process::async_pipe> pipe, boost::int32_t sourceFd, boost::int32_t sinkFd) :
        pipe_(pipe),
        sourceFd_(sourceFd),
        sinkFd_(sinkFd)
    {}

/*
    template<typename Executor>
    void on_setup(Executor& exec)
    {
        std::cout << "Calling on_setup" << std::endl;
    }

    template<typename Executor>
    void on_exec_setup(Executor& exec)
    {
        std::cout << "Calling on_exec_setup" << std::endl;
        if (fcntl(sourceFd_, F_SETFD, 0) != 0)
        {
            std::cout << "Error clearing FD_CLOEXEC bit on source" << std::endl;
        }
        if (fcntl(sinkFd_, F_SETFD, 0) != 0)
        {
            std::cout << "Error clearing FD_CLOEXEC bit on sink" << std::endl;
        }
    }
*/

    template<typename Executor>
    std::function<void(int, const std::error_code&)> on_exit_handler(Executor& exec)
    {
        return [&](int exit_code, const std::error_code& ec)
        {
            std::cout << "Exit code: " << exit_code << std::endl;
            auto pipe = this->pipe_.lock();
            if (pipe)
            {
                pipe->close();
            }
        };
    }

private:
    std::weak_ptr<boost::process::async_pipe> pipe_;
    boost::int32_t sourceFd_;
    boost::int32_t sinkFd_;
};

/*
class Handler : public boost::process::extend::handler
{
public:
    template<typename Executor>
    void on_setup(Executor& exec)
    {
        std::cout << "Calling on_setup" << std::endl;
    }
};
*/

void onData(const boost::system::error_code& ec, size_t bytesTransferred, boost::asio::streambuf& buffer, boost::process::async_pipe& pipe, std::ostringstream& output)
{
    if (!ec)
    {
        if (bytesTransferred > 0)
        {
            boost::asio::streambuf::const_buffers_type buffers = buffer.data();
            std::string data(static_cast<const char*>(buffers.data()), bytesTransferred);
            buffer.consume(bytesTransferred);
            boost::algorithm::trim(data);
            output << data << std::endl;
        }

        boost::asio::async_read_until(pipe, buffer, "\n", boost::bind(&onData, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred, boost::ref(buffer), boost::ref(pipe), boost::ref(output)));
    }
    else if (ec == boost::asio::error::eof)
    {
        // output all the gathered data
        std::cout << "Gathered output:" << std::endl;
        std::cout << output.str() << std::endl;
    }
    else if (ec != boost::system::errc::operation_canceled &&
            ec != boost::asio::error::operation_aborted)
    {
        std::cout << "Got an error: " << ec.message() << std::endl;
    }
}

void onExit(const boost::system::error_code& ec, int exitCode, boost::process::async_pipe& pipe)
{
    std::cout << "Exit code: " << exitCode << std::endl;
    if (ec)
    {
        std::cout << "Error message: " << ec.message() << std::endl;
    }
    pipe.cancel();
}

void myfunc(boost::asio::io_context& io_context, boost::asio::streambuf& buffer, boost::process::async_pipe& pipe, std::ostringstream& output)
{
    std::cout << "Running myfunc" << std::endl;

    std::ostringstream cmdStream;
    cmdStream << "sudo /usr/bin/apt-get -y -o APT::Status-Fd=" << pipe.native_source() << " install apt-doc";
    //cmdStream << "sudo /usr/bin/apt-get -y -o APT::Status-Fd=2 install apt-doc";
    std::cout << cmdStream.str() << std::endl;

    // fork the process
    boost::process::async_system(io_context, boost::bind(&onExit, boost::asio::placeholders::error, _2, boost::ref(pipe)), cmdStream.str(),
        boost::process::std_err > pipe, boost::process::std_out > boost::process::null);

    // read the pipe
    boost::asio::async_read_until(pipe, buffer, "\n", boost::bind(&onData, boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred, boost::ref(buffer), boost::ref(pipe), boost::ref(output)));
}

int main(int argc, char* argv[])
{
    std::cout << "Hello world!" << std::endl;
    boost::asio::io_context io_context;
    boost::asio::streambuf buffer;
    auto pipe = std::make_shared<boost::process::async_pipe>(io_context);
    //boost::process::async_pipe pipe(io_context);
    std::ostringstream cmd, output;

    auto sourceFd = pipe->native_source();
    auto sinkFd = pipe->native_sink();
    //cmd << "sudo /usr/bin/apt-get -y -o APT::Status-Fd=" << sourceFd << " install apt-doc";
    cmd << "sudo /usr/bin/apt-get -y -o APT::Status-Fd=2 install apt-doc";
    std::cout << cmd.str() << std::endl;
    //io_context.post(boost::bind(&myfunc, boost::ref(io_context), boost::ref(buffer), boost::ref(pipe), boost::ref(output)));

    /*
    if (fcntl(sourceFd, F_SETFD, 0) != 0)
    {
        std::cout << "Error clearing FD_CLOEXEC bit on pipe" << std::endl;
    }

    if (fcntl(sinkFd, F_SETFD, FD_CLOEXEC) != 0)
    {
        std::cout << "Error setting FD_CLOEXEC bit on pipe" << std::endl;
    }
    */

    AsyncHandler handler(pipe, sourceFd, sinkFd);
    //boost::process::child child(cmd.str(), handler, io_context);
    boost::process::child child(cmd.str(), handler, boost::process::std_err > (*pipe), boost::process::std_out > boost::process::null, io_context);

    // read the pipe
    boost::asio::async_read_until(*pipe, buffer, "\n", boost::bind(&onData, boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred, boost::ref(buffer), boost::ref(*pipe), boost::ref(output)));

    io_context.run();

    std::cout << "All done!" << std::endl;
}
