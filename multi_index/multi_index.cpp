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
#include <DeviceUpdateInfo.hpp>

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

    DeviceUpdateInfoSet infoSet;
    DeviceUpdateInfo dui1(1);
    dui1.ipAddress("192.168.1.10");

    DeviceUpdateInfo dui2(2);
    dui2.ipAddress("192.168.1.9");

    DeviceUpdateInfo dui3(3);
    dui3.ipAddress("192.168.1.8");

    infoSet.insert(dui1);
    infoSet.insert(dui2);
    infoSet.insert(dui3);

    std::cout << std::endl;
    std::cout << "indexed by device id" << std::endl;
    BOOST_FOREACH(const DeviceUpdateInfo& info, infoSet.get<by_device>())
    {
        std::cout << "[device id: " << info.deviceId() << "][ip address: " << info.ipAddress() << "][binding id: " << info.bindingId() << "]" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "indexed by ip address" << std::endl;
    BOOST_FOREACH(const DeviceUpdateInfo& info, infoSet.get<by_address>())
    {
        std::cout << "[device id: " << info.deviceId() << "][ip address: " << info.ipAddress() << "][binding id: " << info.bindingId() << "]" << std::endl;
    }

    DeviceUpdateInfoSet::index<by_address>::type::iterator addrIt = infoSet.get<by_address>().find("192.168.1.9");
    if (addrIt != infoSet.get<by_address>().end())
    {
        DeviceUpdateInfo foundInfo = *addrIt;
        foundInfo.ipAddress("192.168.1.5");
        infoSet.get<by_address>().replace(addrIt, foundInfo);

        std::cout << std::endl;
        std::cout << "indexed by device id (after replacement)" << std::endl;
        BOOST_FOREACH(const DeviceUpdateInfo& info, infoSet.get<by_device>())
        {
            std::cout << "[device id: " << info.deviceId() << "][ip address: " << info.ipAddress() << "][binding id: " << info.bindingId() << "]" << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "finished" << std::endl;

    return 0;
}
