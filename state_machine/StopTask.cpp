/*
 * StopTask.cpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#include <iostream>

#include <StopTask.hpp>
#include <StateMachine.hpp>

StopTask::StopTask(StateMachine& machine) :
    Task(machine)
{
    std::cout << "[StopTask] constructor" << std::endl;
}

StopTask::~StopTask()
{
    std::cout << "[StopTask] destructor" << std::endl;
}

void StopTask::execute()
{
    std::cout << "[StopTask] execute" << std::endl;
    machine_.taskComplete(boost::shared_ptr<Task>());
}
