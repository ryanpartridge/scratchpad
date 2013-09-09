/*
 * State.hpp
 *
 *  Created on: Sep 6, 2013
 *      Author: rpartridge
 */

#ifndef STATE_HPP_
#define STATE_HPP_

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

class StateMachine;

class State
{
public:
    State(StateMachine& machine);
    virtual ~State();

    virtual void execute() = 0;

protected:
    boost::asio::io_service& io_service();
    void stateComplete(boost::shared_ptr<State> nextState);
    StateMachine& machine_;
};

#endif /* STATE_HPP_ */
