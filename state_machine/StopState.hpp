/*
 * StopState.hpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#ifndef STOPSTATE_HPP_
#define STOPSTATE_HPP_

#include <State.hpp>

class StopState :
    public State
{
public:
    StopState(StateMachine& machine);
    virtual ~StopState();

    virtual void execute();
};

#endif /* STOPSTATE_HPP_ */
