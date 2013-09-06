/*
 * Task.hpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#ifndef TASK_HPP_
#define TASK_HPP_

#include <boost/shared_ptr.hpp>

class StateMachine;

class Task
{
public:
    Task(StateMachine& machine);
    virtual ~Task();

    virtual void execute() = 0;

protected:
    StateMachine& machine_;
};

#endif /* TASK_HPP_ */
