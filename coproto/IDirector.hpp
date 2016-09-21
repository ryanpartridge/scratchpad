/*
 * IDirector.hpp
 *
 *  Created on: Sep 16, 2016
 *      Author: rpartridge
 */

#ifndef IDIRECTOR_HPP_
#define IDIRECTOR_HPP_

#include <string>
#include <boost/asio/spawn.hpp>

class IDirector
{
protected:
    IDirector() {};
    virtual ~IDirector() {};

public:
    virtual std::string getValue(const std::string& name) = 0;
    virtual std::string getValue(const std::string& name, boost::asio::yield_context yield) = 0;
};

#endif /* IDIRECTOR_HPP_ */
