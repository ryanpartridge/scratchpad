/*
 * Task.cpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#include <iostream>

#include <Task.hpp>

Task::Task(StateMachine& machine) :
    machine_(machine)
{
    std::cout << "[Task] constructor" << std::endl;
}

Task::~Task()
{
    std::cout << "[Task] destructor" << std::endl;
}
