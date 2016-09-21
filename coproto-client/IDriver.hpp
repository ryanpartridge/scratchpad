/*
 * IDriver.hpp
 *
 *  Created on: Sep 19, 2016
 *      Author: rpartridge
 */

#ifndef IDRIVER_HPP_
#define IDRIVER_HPP_

#include <boost/asio/spawn.hpp>

class IDriver
{
protected:
    IDriver() {};
    virtual ~IDriver() {};

public:
    virtual std::size_t getCount(boost::asio::yield_context yield) = 0;
};

#endif /* IDRIVER_HPP_ */
