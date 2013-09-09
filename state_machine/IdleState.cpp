/*
 * IdleState.cpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#include <iostream>

#include <IdleState.hpp>
#include <StopState.hpp>
#include <StateMachine.hpp>

IdleState::IdleState(StateMachine& machine) :
    State(machine)
{
    std::cout << "[IdleState] constructor" << std::endl;
}

IdleState::~IdleState()
{
    std::cout << "[IdleState] destructor" << std::endl;
}

void IdleState::execute()
{
    std::cout << "[IdleState] execute" << std::endl;
    try
    {
        std::string message = machine_.popMessage();
        std::cout << "[IdleState] got message: " << message << std::endl;
        if (message == "stop")
        {
            stateComplete(boost::shared_ptr<State>(new StopState(machine_)));
        }
        else
        {
            stateComplete(boost::shared_ptr<State>(new IdleState(machine_)));
        }
    }
    catch (const boost::thread_interrupted& )
    {
        std::cout << "[IdleState] thread has been interrupted" << std::endl;
//        machine_.stateComplete(boost::shared_ptr<Task>());
    }
}
