#include <iostream>
#include <sstream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/process/async_pipe.hpp>
#include <boost/process/async_system.hpp>
#include <boost/process/io.hpp>
#include <boost/algorithm/string/trim.hpp>

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
    else
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
    //cmdStream << "sudo /usr/bin/apt-get -y -o APT::Status-Fd=" << pipe.native_source() << " install apt-doc";
    cmdStream << "sudo /usr/bin/apt-get -y -o APT::Status-Fd=2 install apt-doc";
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
    boost::process::async_pipe pipe(io_context);
    std::ostringstream output;

    io_context.post(boost::bind(&myfunc, boost::ref(io_context), boost::ref(buffer), boost::ref(pipe), boost::ref(output)));
    io_context.run();
    std::cout << "All done!" << std::endl;
}
