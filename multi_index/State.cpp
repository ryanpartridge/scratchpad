/*
 * State.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: rpartridge
 */

#include <boost/make_shared.hpp>

#include <State.hpp>

State::State() :
    name_("State")
{
}

State::~State()
{
}

boost::shared_ptr<State> State::createState()
{
    return boost::make_shared<State>();
}

const std::string& State::name() const
{
    return name_;
}
