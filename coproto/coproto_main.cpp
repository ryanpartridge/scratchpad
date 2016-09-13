/*
 * coproto.cpp
 *
 *  Created on: Aug 9, 2016
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/cstdint.hpp>

#include <coproto_handle.hpp>

using namespace std;

void useTimer(boost::asio::io_service& io_service, boost::asio::yield_context yld)
{
    boost::system::error_code ec;
    boost::asio::steady_timer timer(io_service, boost::chrono::seconds(2));
    cout << "staring timer 1" << endl;
    timer.async_wait(yld[ec]);
    if (!ec)
    {
        cout << "timer 1 expired normally" << endl;
    }
    else
    {
        cout << "timer 1 expired with error: " << ec.message() << endl;
    }
    cout << "done with timer 1" << endl;
}

void useDo(boost::asio::io_service& io_service, boost::asio::yield_context yld)
{
    boost::system::error_code ec;
    coproto_handle co_handle(io_service);
    cout << "starting do operation" << endl;
    int value = co_handle.async_do(yld[ec]);
    if (!ec)
    {
        cout << "do returned normally [value: " << value << "]" << endl;
    }
    else
    {
        cout << "do returned with error: " << ec.message() << endl;
    }
    cout << "done with do operation" << endl;
}

void do_handler(const boost::system::error_code& ec, int value)
{
    cout << "do_handler called [value: " << value << "]" << endl;
}

void useSignal(boost::asio::io_service& io_service, boost::asio::yield_context yld)
{
    boost::system::error_code ec;
    boost::asio::signal_set signal(io_service);
    signal.add(SIGHUP);
    signal.add(SIGUSR1);
    signal.add(SIGUSR2);
    signal.add(SIGQUIT);
    cout << "starting signal wait" << endl;

    boost::uint32_t sigNumber = signal.async_wait(yld[ec]);
    if (!ec)
    {
        cout << "handling signal: " << sigNumber << " normally" << endl;
    }
    else
    {
        cout << "signal wait returned with error: " << ec.message() << endl;
    }
    cout << "done with signal wait" << endl;
}

void handle_connection(boost::shared_ptr<boost::asio::ip::tcp::socket> connection, boost::asio::io_service& io_service, boost::asio::yield_context yield)
{
    boost::system::error_code ec;
    boost::asio::streambuf buffer;
    cout << "waiting for a packet" << endl;
    std::size_t bytes_read = boost::asio::async_read_until(*connection, buffer, "\r\n", yield[ec]);
    if (!ec)
    {

    }
}

void accept_connection(boost::asio::ip::tcp::acceptor& acceptor, boost::asio::io_service& io_service, boost::asio::yield_context yield)
{
    boost::system::error_code ec;
    while(!ec)
    {
        boost::shared_ptr<boost::asio::ip::tcp::socket> listen_socket = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(io_service));
        acceptor.async_accept(*listen_socket, yield[ec]);
        if (!ec)
        {
            boost::asio::spawn(io_service, boost::bind(&handle_connection, listen_socket, boost::ref(io_service), _1));
        }
    }
}

int main(int argc, char* argv[])
{
    cout << "entering main" << endl;

    cout << "setting up the acceptor socket" << endl;
    boost::system::error_code ec;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::loopback(), 9876);
    boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint, true);
    boost::asio::spawn(io_service, boost::bind(&accept_connection, boost::ref(acceptor), boost::ref(io_service), _1));
//    boost::asio::spawn(io_service, boost::bind(&useTimer, boost::ref(io_service), _1));
//    boost::asio::spawn(io_service, boost::bind(&useSignal, boost::ref(io_service), _1));
//    boost::asio::spawn(io_service, boost::bind(&useDo, boost::ref(io_service), _1));
//    coproto_handle co_handle(io_service);
//    co_handle.async_do(&do_handler);
    cout << "calling run()" << endl;
    io_service.run();
    cout << "run() has finished" << endl;
    cout << "exiting main" << endl;
    return 0;
}
