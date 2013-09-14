/*
 * InitState.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: rpartridge
 */

#include <boost/make_shared.hpp>

#include <InitState.hpp>

InitState::InitState() :
    State()
{
    name_ = "InitState";
}

InitState::~InitState()
{
    // TODO Auto-generated destructor stub
}

boost::shared_ptr<State> InitState::createState()
{
    return boost::make_shared<InitState>();
}
