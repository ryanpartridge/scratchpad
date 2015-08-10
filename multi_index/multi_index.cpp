/*
 * multi_index.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <vector>
#include <utility>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>

#include <State.hpp>
#include <InitState.hpp>
#include <StopState.hpp>
#include <DeviceUpdateInfo.hpp>
#include <DeviceType.hpp>

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
    BOOST_FOREACH(const DeviceUpdateInfo& info, infoSet.get<DeviceUpdateInfo::by_device>())
    {
        std::cout << "[device id: " << info.deviceId() << "][ip address: " << info.ipAddress() << "][binding id: " << info.bindingId() << "]" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "indexed by ip address" << std::endl;
    BOOST_FOREACH(const DeviceUpdateInfo& info, infoSet.get<DeviceUpdateInfo::by_address>())
    {
        std::cout << "[device id: " << info.deviceId() << "][ip address: " << info.ipAddress() << "][binding id: " << info.bindingId() << "]" << std::endl;
    }

    DeviceUpdateInfoSet::index<DeviceUpdateInfo::by_address>::type::iterator addrIt = infoSet.get<DeviceUpdateInfo::by_address>().find("192.168.1.9");
    if (addrIt != infoSet.get<DeviceUpdateInfo::by_address>().end())
    {
        DeviceUpdateInfo foundInfo = *addrIt;
        foundInfo.ipAddress("192.168.1.5");
        infoSet.get<DeviceUpdateInfo::by_address>().replace(addrIt, foundInfo);

        std::cout << std::endl;
        std::cout << "indexed by device id (after replacement)" << std::endl;
        BOOST_FOREACH(const DeviceUpdateInfo& info, infoSet.get<DeviceUpdateInfo::by_device>())
        {
            std::cout << "[device id: " << info.deviceId() << "][ip address: " << info.ipAddress() << "][binding id: " << info.bindingId() << "]" << std::endl;
        }
    }

    addrIt = infoSet.get<DeviceUpdateInfo::by_address>().find("192.168.1.5");
    if (addrIt != infoSet.get<DeviceUpdateInfo::by_address>().end())
    {
        infoSet.get<DeviceUpdateInfo::by_address>().modify(addrIt, DeviceUpdateInfo::update_ip_address("192.168.1.20"));
        std::cout << std::endl;
        std::cout << "indexed by device id (after modification)" << std::endl;
        BOOST_FOREACH(const DeviceUpdateInfo& info, infoSet.get<DeviceUpdateInfo::by_device>())
        {
            std::cout << "[device id: " << info.deviceId() << "][ip address: " << info.ipAddress() << "][binding id: " << info.bindingId() << "]" << std::endl;
        }
    }

    addrIt = infoSet.get<DeviceUpdateInfo::by_address>().find("192.168.1.20");
    const DeviceUpdateInfo& refDui = *addrIt;
    std::cout << std::endl;
    std::cout << "iterator access by const ref" << std::endl;
    std::cout << "[device id: " << refDui.deviceId() << "][ip address: " << refDui.ipAddress() << "][binding id: " << refDui.bindingId() << "]" << std::endl;


    DeviceUpdateInfoPtrSet spInfoSet;
    DeviceUpdateInfo::Ptr spDui1 = boost::make_shared<DeviceUpdateInfo>(1);
    spDui1->ipAddress("192.168.1.10");

    DeviceUpdateInfo::Ptr spDui2 = boost::make_shared<DeviceUpdateInfo>(2);
    spDui2->ipAddress("192.168.1.9");

    DeviceUpdateInfo::Ptr spDui3 = boost::make_shared<DeviceUpdateInfo>(3);
    spDui3->ipAddress("192.168.1.8");

    spInfoSet.insert(spDui1);
    spInfoSet.insert(spDui2);
    spInfoSet.insert(spDui3);

    std::cout << std::endl;
    std::cout << "(sp) indexed by device id" << std::endl;
    BOOST_FOREACH(const DeviceUpdateInfo::Ptr info, spInfoSet.get<DeviceUpdateInfo::by_device>())
    {
        std::cout << "[device id: " << info->deviceId() << "][ip address: " << info->ipAddress() << "][binding id: " << info->bindingId() << "]" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "(sp) indexed by ip address" << std::endl;
    BOOST_FOREACH(const DeviceUpdateInfo::Ptr info, spInfoSet.get<DeviceUpdateInfo::by_address>())
    {
        std::cout << "[device id: " << info->deviceId() << "][ip address: " << info->ipAddress() << "][binding id: " << info->bindingId() << "]" << std::endl;
    }

    DeviceUpdateInfoPtrSet::index<DeviceUpdateInfo::by_address>::type::iterator spAddrIt = spInfoSet.get<DeviceUpdateInfo::by_address>().find("192.168.1.9");
    if (spAddrIt != spInfoSet.get<DeviceUpdateInfo::by_address>().end())
    {
        DeviceUpdateInfo::Ptr foundInfo = *spAddrIt;
        foundInfo->ipAddress("192.168.1.5");
        spInfoSet.get<DeviceUpdateInfo::by_address>().replace(spAddrIt, foundInfo);

        std::cout << std::endl;
        std::cout << "(sp) indexed by device id (after replacement)" << std::endl;
        BOOST_FOREACH(const DeviceUpdateInfo::Ptr info, spInfoSet.get<DeviceUpdateInfo::by_device>())
        {
            std::cout << "[device id: " << info->deviceId() << "][ip address: " << info->ipAddress() << "][binding id: " << info->bindingId() << "]" << std::endl;
        }
    }

    DeviceUpdateInfo::Ptr spDui4 = boost::make_shared<DeviceUpdateInfo>(3);
    spDui4->ipAddress("192.168.1.50");
    std::pair<DeviceUpdateInfoPtrSet::iterator, bool> p = spInfoSet.insert(spDui4);
    if (!p.second)
    {
        std::cout << std::endl;
        std::cout << "did not insert" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "(sp) indexed by device id (after duplicate id insert)" << std::endl;
    BOOST_FOREACH(const DeviceUpdateInfo::Ptr info, spInfoSet.get<DeviceUpdateInfo::by_device>())
    {
        std::cout << "[device id: " << info->deviceId() << "][ip address: " << info->ipAddress() << "][binding id: " << info->bindingId() << "]" << std::endl;
    }

    DeviceTypes deviceTypes;
    DeviceType::Ptr dt = boost::make_shared<DeviceType>("hc800", true, 1.19278);
    deviceTypes.insert(dt);

    dt = boost::make_shared<DeviceType>("hc250", false, 13.8862);
    deviceTypes.insert(dt);

//    dt = boost::make_shared<DeviceType>("c5", true, 11.9274);
    dt = boost::make_shared<DeviceType>("c5", true, 0.0);
    deviceTypes.insert(dt);

    dt = boost::make_shared<DeviceType>("glassedge10", true, 329.868);
    deviceTypes.insert(dt);

//  dt = boost::make_shared<DeviceType>("speakerpoint", false, 0.437582);
    dt = boost::make_shared<DeviceType>("speakerpoint", false, 0.0);
    deviceTypes.insert(dt);

    std::cout << std::endl;
    std::cout << "device types by name" << std::endl;
    BOOST_FOREACH(DeviceType::Ptr devType, deviceTypes)
    {
        std::cout << "[device name: " << devType->name() << "][ratio: " << devType->ratio() << "][marked: " << (devType->markedForDownload() ? "true" : "false") << "]" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "device types by marked ratio" << std::endl;
    DeviceTypeByMarkedRatio::iterator ratioIt(deviceTypes.get<DeviceType::by_marked_ratio>().begin());
    DeviceTypeByMarkedRatio::iterator ratioEnd(deviceTypes.get<DeviceType::by_marked_ratio>().end());
    BOOST_FOREACH(DeviceType::Ptr devType, std::make_pair(ratioIt, ratioEnd))
    {
        std::cout << "[device name: " << devType->name() << "][ratio: " << devType->ratio() << "][marked: " << (devType->markedForDownload() ? "true" : "false") << "]" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "device types marked forward ratio" << std::endl;
    ratioIt = deviceTypes.get<DeviceType::by_marked_ratio>().upper_bound(boost::make_tuple(true, 0.0));
    ratioEnd = deviceTypes.get<DeviceType::by_marked_ratio>().upper_bound(true);
    BOOST_FOREACH(DeviceType::Ptr devType, std::make_pair(ratioIt, ratioEnd))
    {
        std::cout << "[device name: " << devType->name() << "][ratio: " << devType->ratio() << "][marked: " << (devType->markedForDownload() ? "true" : "false") << "]" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "device types marked backward ratio" << std::endl;
    DeviceTypeByMarkedRatio::reverse_iterator reverseRatioIt(deviceTypes.get<DeviceType::by_marked_ratio>().upper_bound(true));
    DeviceTypeByMarkedRatio::reverse_iterator reverseRatioEnd(deviceTypes.get<DeviceType::by_marked_ratio>().upper_bound(boost::make_tuple(true, 0.0)));
    BOOST_FOREACH(DeviceType::Ptr devType, std::make_pair(reverseRatioIt, reverseRatioEnd))
    {
        std::cout << "[device name: " << devType->name() << "][ratio: " << devType->ratio() << "][marked: " << (devType->markedForDownload() ? "true" : "false") << "]" << std::endl;
    }


    std::cout << std::endl;
    std::cout << "finished" << std::endl;

    return 0;
}
