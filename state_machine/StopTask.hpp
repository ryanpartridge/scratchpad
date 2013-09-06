/*
 * StopTask.hpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#ifndef STOPTASK_HPP_
#define STOPTASK_HPP_

#include <Task.hpp>

class StopTask :
    public Task
{
public:
    StopTask(StateMachine& machine);
    virtual ~StopTask();

    virtual void execute();
};

#endif /* STOPTASK_HPP_ */
