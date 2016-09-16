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
#include <boost/algorithm/string.hpp>

#include <coproto_handle.hpp>
#include <QueueManager.hpp>

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

void handle_request(const std::string& request, boost::shared_ptr<boost::asio::ip::tcp::socket> connection, boost::asio::io_service& io_service, boost::asio::yield_context yield)
{
    boost::system::error_code ec;
    std::ostringstream responseStream;
    boost::asio::steady_timer timer(io_service, boost::chrono::seconds(10));
    timer.async_wait(yield[ec]);
    if (request == "a")
    {
        cout << "handling response to 'a'" << endl;
        responseStream << "resp:" << 1 << "\r\n";
        std::string response = responseStream.str();
        boost::asio::async_write(*connection, boost::asio::buffer(response.data(), response.size()), yield[ec]);
    }
}

void handle_connection(boost::shared_ptr<boost::asio::ip::tcp::socket> connection, boost::asio::io_service& io_service, boost::asio::yield_context yield)
{
    boost::system::error_code ec;
    boost::asio::streambuf buffer;
    cout << "waiting for a packet" << endl;
    std::size_t bytes_read = 0;
    while ((bytes_read = boost::asio::async_read_until(*connection, buffer, "\r\n", yield[ec])) > 0 && !ec)
    {
        boost::asio::streambuf::const_buffers_type buffers = buffer.data();
        std::string data(boost::asio::buffers_begin(buffers), boost::asio::buffers_begin(buffers) + bytes_read);
        buffer.consume(bytes_read);
        boost::algorithm::trim(data);
        cout << "incoming message: " << data << endl;
        if (boost::algorithm::starts_with(data, "req:"))
        {
            data = data.substr(4);
            boost::algorithm::trim_left(data);
            boost::asio::spawn(io_service, boost::bind(&handle_request, data, connection, boost::ref(io_service), _1));
        }
        else if (boost::algorithm::starts_with(data, "resp:"))
        {
            data = data.substr(4);
            boost::algorithm::trim_left(data);
            cout << "don't now how to handle responses yet" << endl;
        }
    }
    cout << "connection closed" << endl;
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
    boost::system::error_code ec;
    boost::asio::io_service io_service;


    cout << "setting up the acceptor socket" << endl;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::loopback(), 9876);
    boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint, true);

//    boost::asio::spawn(io_service, boost::bind(&accept_connection, boost::ref(acceptor), boost::ref(io_service), _1));
//    boost::asio::spawn(io_service, boost::bind(&useTimer, boost::ref(io_service), _1));
//    boost::asio::spawn(io_service, boost::bind(&useSignal, boost::ref(io_service), _1));
    boost::asio::spawn(io_service, boost::bind(&useDo, boost::ref(io_service), _1));
//    coproto_handle co_handle(io_service);
//    co_handle.async_do(&do_handler);

    typename QueueManager<std::string>::queue_type& in_queue = QueueManager<std::string>::get_in_queue();
    typename QueueManager<std::string>::queue_type& out_queue = QueueManager<std::string>::get_out_queue();

    cout << "calling run()" << endl;
    io_service.run();
    cout << "run() has finished" << endl;
    cout << "exiting main" << endl;
    return 0;
}
