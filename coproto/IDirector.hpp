/*
 * IDirector.hpp
 *
 *  Created on: Sep 16, 2016
 *      Author: rpartridge
 */

#ifndef IDIRECTOR_HPP_
#define IDIRECTOR_HPP_

#include <string>

class IDirector
{
protected:
    IDirector() {};
    virtual ~IDirector() {};

    virtual std::string getValue(const std::string& name) = 0;
};

#endif /* IDIRECTOR_HPP_ */
