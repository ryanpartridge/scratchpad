/*
 * multi_index.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: rpartridge
 */

#include <State.hpp>
#include <InitState.hpp>

int main(int argc, char* argv[])
{
    StateSet entries;
    entries.insert(StateEntry(INIT, "init", &InitState::createState));

    return 0;
}
