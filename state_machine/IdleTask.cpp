/*
 * IdleTask.cpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#include <iostream>

#include <IdleTask.hpp>
#include <StopTask.hpp>
#include <StateMachine.hpp>

IdleTask::IdleTask(StateMachine& machine) :
    Task(machine)
{
    std::cout << "[IdleTask] constructor" << std::endl;
}

IdleTask::~IdleTask()
{
    std::cout << "[IdleTask] destructor" << std::endl;
}

void IdleTask::execute()
{
    std::cout << "[IdleTask] execute" << std::endl;
    try
    {
        std::string message = machine_.popMessage();
        std::cout << "[IdleTask] got message: " << message << std::endl;
        if (message == "stop")
        {
            taskComplete(boost::shared_ptr<Task>(new StopTask(machine_)));
        }
        else
        {
            taskComplete(boost::shared_ptr<Task>(new IdleTask(machine_)));
        }
    }
    catch (const boost::thread_interrupted& )
    {
        std::cout << "[IdleTask] thread has been interrupted" << std::endl;
//        machine_.taskComplete(boost::shared_ptr<Task>());
    }
}
