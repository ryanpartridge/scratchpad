/*
 * multi_index.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/shared_ptr.hpp>

#include <State.hpp>
#include <InitState.hpp>
#include <StopState.hpp>

int main(int argc, char* argv[])
{
    StateSet entries;
    StateCreatorFunction initFunc(InitState::createState);
    entries.insert(StateEntry(INIT, "init", initFunc));

    StateCreatorFunction stopFunc(StopState::createState);
    entries.insert(StateEntry(STOP, "stop", stopFunc));

    StateSet::iterator it = entries.find(INIT);
    boost::shared_ptr<State> s1 = it->creatorFunction_();

    it = entries.find(STOP);
    boost::shared_ptr<State> s2 = it->creatorFunction_();

    std::cout << "State name: " << s1->name() << std::endl;
    std::cout << "State name: " << s2->name() << std::endl;

    return 0;
}
