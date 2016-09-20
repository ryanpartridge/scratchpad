/*
 * IDriver.hpp
 *
 *  Created on: Sep 19, 2016
 *      Author: rpartridge
 */

#ifndef IDRIVER_HPP_
#define IDRIVER_HPP_

#include <string>

class IDriver
{
protected:
    IDriver() {};
    virtual ~IDriver() {};

    virtual std::size_t getCount() = 0;
};

#endif /* IDRIVER_HPP_ */
