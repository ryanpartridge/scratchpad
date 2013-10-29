/*
 * multi_index.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

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

    StateEntry entryArray[] =
        {
            StateEntry(INIT, "init", initFunc),
            StateEntry(STOP, "stop", stopFunc)
        };

    StateSet entires2(entryArray, entryArray + (sizeof(entryArray) / sizeof(entryArray[0])));

    StateSet::iterator it = entries.find(INIT);
    boost::shared_ptr<State> s1 = it->creatorFunction_();

    StateSet::nth_index<1>::type::iterator it2 = entries.get<1>().find("stop");
    boost::shared_ptr<State> s2 = it2->creatorFunction_();

    std::cout << "State name: " << s1->name() << std::endl;
    std::cout << "State name: " << s2->name() << std::endl;

    BOOST_FOREACH(const StateEntry& entry, entires2)
    {
        std::cout << entry.umStateName_ << std::endl;
    }

    return 0;
}
