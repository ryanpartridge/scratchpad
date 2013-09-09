/*
 * IdleState.hpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#ifndef IDLESTATE_HPP_
#define IDLESTATE_HPP_

#include <State.hpp>

class IdleState :
    public State
{
public:
    IdleState(StateMachine& machine);
    virtual ~IdleState();

    virtual void execute();
};

#endif /* IDLESTATE_HPP_ */
