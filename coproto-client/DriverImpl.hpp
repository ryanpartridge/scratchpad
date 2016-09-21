/*
 * DriverImpl.hpp
 *
 *  Created on: Sep 19, 2016
 *      Author: rpartridge
 */

#ifndef DRIVERIMPL_HPP_
#define DRIVERIMPL_HPP_

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/shared_ptr.hpp>

#include <IDriver.hpp>
#include <QueueManager.hpp>
#include <DirectorStub.hpp>

class DriverImpl : public IDriver
{
public:
    DriverImpl();
    virtual ~DriverImpl();

    virtual std::size_t getCount(boost::asio::yield_context yield);
    void start();

private:
    typedef QueueManager<std::string> QueueOwner;
    typedef QueueOwner::queue_type QueueType;

    void startConnection(boost::asio::yield_context yield);
    void handleIncoming(boost::asio::yield_context yield);
    void handleConnection(boost::shared_ptr<boost::asio::ip::tcp::socket> connection, boost::asio::yield_context yield);
    void serviceOutQueue(boost::asio::yield_context yield);
    void dispatchRequest(const std::string& request, boost::asio::yield_context yield);
    void startRequestTimer(boost::asio::yield_context yield);

    std::size_t driverId_;
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::endpoint endpoint_;
    boost::asio::ip::tcp::socket socket_;
    QueueType& inQueue_;
    QueueType& outQueue_;
    DirectorStub director_;

    static std::size_t nextDriverId_;
};

#endif /* DRIVERIMPL_HPP_ */
