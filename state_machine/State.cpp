/*
 * State.cpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/ref.hpp>

#include <State.hpp>
#include <StateMachine.hpp>

State::State(StateMachine& machine) :
    machine_(machine)
{
    std::cout << "[Task] constructor" << std::endl;
}

State::~State()
{
    std::cout << "[Task] destructor" << std::endl;
}

boost::asio::io_service& State::io_service()
{
    return machine_.io_service();
}

void State::stateComplete(boost::shared_ptr<State> nextState)
{
    io_service().post(boost::bind(&StateMachine::handleStateComplete, boost::ref(machine_), nextState));
}
