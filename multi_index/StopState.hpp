/*
 * StopState.hpp
 *
 *  Created on: Sep 13, 2013
 *      Author: rpartridge
 */

#ifndef STOPSTATE_HPP_
#define STOPSTATE_HPP_

#include <State.hpp>

class StopState:
    public State
{
public:
    StopState();
    virtual ~StopState();

    static boost::shared_ptr<State> createState();
};

#endif /* STOPSTATE_HPP_ */
