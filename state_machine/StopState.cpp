/*
 * StopState.cpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#include <iostream>

#include <StopState.hpp>
#include <StateMachine.hpp>

StopState::StopState(StateMachine& machine) :
    State(machine)
{
    std::cout << "[StopTask] constructor" << std::endl;
}

StopState::~StopState()
{
    std::cout << "[StopTask] destructor" << std::endl;
}

void StopState::execute()
{
    std::cout << "[StopTask] execute" << std::endl;
    stateComplete(boost::shared_ptr<State>());
}
