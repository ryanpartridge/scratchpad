/*
 * DriverStub.hpp
 *
 *  Created on: Sep 20, 2016
 *      Author: rpartridge
 */

#ifndef DRIVERSTUB_HPP_
#define DRIVERSTUB_HPP_

#include <boost/asio.hpp>

#include <IDriver.hpp>

class DriverStub : public IDriver
{
public:
    explicit DriverStub(boost::asio::io_service& io_service);
    virtual ~DriverStub();

    virtual std::size_t getCount(boost::asio::yield_context yield);

private:
    boost::asio::io_service& io_service_;
};

#endif /* DRIVERSTUB_HPP_ */
