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
    IDirector() {};
    virtual ~IDirector() {};

    virtual std::string getValue(std::string name) = 0;
};

#endif /* IDIRECTOR_HPP_ */
