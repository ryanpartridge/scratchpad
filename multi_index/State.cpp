/*
 * State.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: rpartridge
 */

#include <boost/make_shared.hpp>

#include <State.hpp>

State::State()
{
    // TODO Auto-generated constructor stub

}

State::~State()
{
    // TODO Auto-generated destructor stub
}

boost::shared_ptr<State> State::createState()
{
    return boost::make_shared<State>();
}
