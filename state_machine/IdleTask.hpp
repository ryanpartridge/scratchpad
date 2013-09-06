/*
 * IdleTask.hpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#ifndef IDLETASK_HPP_
#define IDLETASK_HPP_

#include <Task.hpp>

class IdleTask :
    public Task
{
public:
    IdleTask(StateMachine& machine);
    virtual ~IdleTask();

    virtual void execute();
};

#endif /* IDLETASK_HPP_ */
