/*
 * DirectorImpl.hpp
 *
 *  Created on: Sep 16, 2016
 *      Author: rpartridge
 */

#ifndef DIRECTORIMPL_HPP_
#define DIRECTORIMPL_HPP_

#include <string>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/shared_ptr.hpp>

#include <IDirector.hpp>
#include <QueueManager.hpp>
#include <DriverStub.hpp>

class DirectorImpl : public IDirector
{
public:
    DirectorImpl();
    virtual ~DirectorImpl();

    virtual std::string getValue(const std::string& name, boost::asio::yield_context yield);
    virtual bool isSet(const std::string& name, boost::asio::yield_context yield);
    void start();

private:
    typedef QueueManager<std::string> QueueOwner;
    typedef QueueOwner::queue_type QueueType;

    void startAcceptor(boost::system::error_code& ec);
    void acceptConnection(boost::asio::yield_context yield);
    void handleConnection(boost::shared_ptr<boost::asio::ip::tcp::socket> connection, boost::asio::yield_context yield);
    void serviceOutQueue(boost::shared_ptr<boost::asio::ip::tcp::socket> connection, boost::asio::yield_context yield);
    void dispatchRequest(const std::string& request, boost::asio::yield_context yield);

    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::endpoint endpoint_;
    boost::asio::ip::tcp::acceptor acceptor_;
    QueueType& inQueue_;
    QueueType& outQueue_;
    DriverStub driver_;
};

#endif /* DIRECTORIMPL_HPP_ */
