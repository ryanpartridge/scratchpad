/*
 * update_database.cpp
 *
 *  Created on: Jul 15, 2015
 *      Author: Ryan Partridge
 */

#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

#include <DeviceUpdateInfo.hpp>

int main(int argc, char* argv[])
{
    DeviceUpdateInfoSet infoSet;
    std::pair<DeviceUpdateInfoSet::iterator, bool> insertPair = infoSet.insert(boost::make_shared<DeviceUpdateInfo>(1));
    if (insertPair.second && insertPair.first != infoSet.end())
    {
        infoSet.modify(insertPair.first, DeviceUpdateInfo::update_ip_address("192.168.1.10"));
    }

    insertPair = infoSet.insert(boost::make_shared<DeviceUpdateInfo>(2));
    if (insertPair.second && insertPair.first != infoSet.end())
    {
        infoSet.modify(insertPair.first, DeviceUpdateInfo::update_ip_address("192.168.1.9"));
    }

    insertPair = infoSet.insert(boost::make_shared<DeviceUpdateInfo>(3));
    if (insertPair.second && insertPair.first != infoSet.end())
    {
        infoSet.modify(insertPair.first, DeviceUpdateInfo::update_ip_address("192.168.1.8"));
    }

    std::cout << "indexed by device id" << std::endl;
    BOOST_FOREACH(const DeviceUpdateInfo::Ptr info, infoSet)
    {
        std::cout << "[device id: " << info->deviceId() << "][ip address: " << info->ipAddress() << "][binding id: " << info->bindingId() << "]" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "indexed by ip address" << std::endl;
    BOOST_FOREACH(const DeviceUpdateInfo::Ptr info, infoSet.get<DeviceUpdateInfo::by_address>())
    {
        std::cout << "[device id: " << info->deviceId() << "][ip address: " << info->ipAddress() << "][binding id: " << info->bindingId() << "]" << std::endl;
    }

    DeviceUpdateInfoSet::index<DeviceUpdateInfo::by_address>::type::iterator addrIt = infoSet.get<DeviceUpdateInfo::by_address>().find("192.168.1.9");
    if (addrIt != infoSet.get<DeviceUpdateInfo::by_address>().end())
    {
        infoSet.get<DeviceUpdateInfo::by_address>().modify(addrIt, DeviceUpdateInfo::update_ip_address("192.168.1.20"));
        std::cout << std::endl;
        std::cout << "indexed by device id (after modification)" << std::endl;
        BOOST_FOREACH(const DeviceUpdateInfo::Ptr info, infoSet)
        {
            std::cout << "[device id: " << info->deviceId() << "][ip address: " << info->ipAddress() << "][binding id: " << info->bindingId() << "]" << std::endl;
        }
    }
    else
    {
        std::cout << "did not find" << std::endl;
    }

    DeviceUpdateInfo::Ptr dui4 = boost::make_shared<DeviceUpdateInfo>(3);
    std::pair<DeviceUpdateInfoSet::iterator, bool> p = infoSet.insert(dui4);
    if (!p.second)
    {
        std::cout << std::endl;
        std::cout << "did not insert" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "indexed by device id (after duplicate id insert)" << std::endl;
    BOOST_FOREACH(const DeviceUpdateInfo::Ptr info, infoSet)
    {
        std::cout << "[device id: " << info->deviceId() << "][ip address: " << info->ipAddress() << "][binding id: " << info->bindingId() << "]" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "finished" << std::endl;

    return 0;
}
