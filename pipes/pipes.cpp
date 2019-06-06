#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

void myfunc(boost::asio::io_context& io_context)
{
    std::cout << "Running myfunc" << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "Hello world!" << std::endl;
    boost::asio::io_context io_context;

    io_context.post(boost::bind(&myfunc, boost::ref(io_context)));
    io_context.run();
    std::cout << "All done!" << std::endl;
}
