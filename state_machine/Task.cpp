/*
 * Task.cpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/ref.hpp>

#include <Task.hpp>
#include <StateMachine.hpp>

Task::Task(StateMachine& machine) :
    machine_(machine)
{
    std::cout << "[Task] constructor" << std::endl;
}

Task::~Task()
{
    std::cout << "[Task] destructor" << std::endl;
}

boost::asio::io_service& Task::io_service()
{
    return machine_.io_service();
}

void Task::taskComplete(boost::shared_ptr<Task> nextTask)
{
    io_service().post(boost::bind(&StateMachine::handleTaskComplete, boost::ref(machine_), nextTask));
}
