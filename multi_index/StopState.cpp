/*
 * StopState.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: rpartridge
 */

#include <boost/make_shared.hpp>

#include <StopState.hpp>

StopState::StopState() :
    State()
{
    name_ = "StopState";
}

StopState::~StopState()
{
    // TODO Auto-generated destructor stub
}

boost::shared_ptr<State> StopState::createState()
{
    return boost::make_shared<StopState>();
}
