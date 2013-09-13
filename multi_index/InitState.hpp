/*
 * InitState.hpp
 *
 *  Created on: Sep 13, 2013
 *      Author: rpartridge
 */

#ifndef INITSTATE_HPP_
#define INITSTATE_HPP_

#include <State.hpp>

class InitState :
    public State
{
public:
    InitState();
    virtual ~InitState();

    static boost::shared_ptr<State> createState();
};

#endif /* INITSTATE_HPP_ */
