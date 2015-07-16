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

#include <Device.hpp>
#include <DeviceType.hpp>
#include <Package.hpp>

int main(int argc, char* argv[])
{
    Devices devices;
    std::pair<Devices::iterator, bool> insertPair = devices.insert(boost::make_shared<Device>(1));
    if (insertPair.second && insertPair.first != devices.end())
    {
        devices.modify(insertPair.first, Device::update_ip_address("192.168.1.10"));
    }

    insertPair = devices.insert(boost::make_shared<Device>(2));
    if (insertPair.second && insertPair.first != devices.end())
    {
        devices.modify(insertPair.first, Device::update_ip_address("192.168.1.9"));
    }

    insertPair = devices.insert(boost::make_shared<Device>(3));
    if (insertPair.second && insertPair.first != devices.end())
    {
        devices.modify(insertPair.first, Device::update_ip_address("192.168.1.8"));
    }

    std::cout << "indexed by device id" << std::endl;
    BOOST_FOREACH(const Device::Ptr device, devices)
    {
        std::cout << "[device id: " << device->deviceId() << "][ip address: " << device->ipAddress() << "][binding id: " << device->bindingId() << "]" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "indexed by ip address" << std::endl;
    BOOST_FOREACH(const Device::Ptr device, devices.get<Device::by_address>())
    {
        std::cout << "[device id: " << device->deviceId() << "][ip address: " << device->ipAddress() << "][binding id: " << device->bindingId() << "]" << std::endl;
    }

    Devices::index<Device::by_address>::type::iterator addrIt = devices.get<Device::by_address>().find("192.168.1.9");
    if (addrIt != devices.get<Device::by_address>().end())
    {
        devices.get<Device::by_address>().modify(addrIt, Device::update_ip_address("192.168.1.20"));
        std::cout << std::endl;
        std::cout << "indexed by device id (after modification)" << std::endl;
        BOOST_FOREACH(const Device::Ptr device, devices)
        {
            std::cout << "[device id: " << device->deviceId() << "][ip address: " << device->ipAddress() << "][binding id: " << device->bindingId() << "]" << std::endl;
        }
    }
    else
    {
        std::cout << "did not find" << std::endl;
    }

    Device::Ptr dui4 = boost::make_shared<Device>(3);
    std::pair<Devices::iterator, bool> p = devices.insert(dui4);
    if (!p.second)
    {
        std::cout << std::endl;
        std::cout << "did not insert" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "indexed by device id (after duplicate id insert)" << std::endl;
    BOOST_FOREACH(const Device::Ptr device, devices)
    {
        std::cout << "[device id: " << device->deviceId() << "][ip address: " << device->ipAddress() << "][binding id: " << device->bindingId() << "]" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "finished" << std::endl;

    return 0;
}
