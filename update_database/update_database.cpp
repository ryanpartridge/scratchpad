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

void printDevice(Device::Ptr device)
{
    std::cout << "device - [id: " << device->deviceId()
        << "][device type: " << device->deviceType()
        << "][ip address: " << device->ipAddress()
        << "][binding id: " << device->bindingId()
        << "]" << std::endl;
}

void printDeviceType(DeviceType::Ptr deviceType)
{
    std::cout << "device type - [name: " << deviceType->name()
        << "][target version: " << deviceType->targetVersion()
        << "]" << std::endl;
}

void printPackage(Package::Ptr package)
{
    std::cout << "package - [filename: " << package->filename()
        << "][name: " << package->name()
        << "]" << std::endl;
}

void createDevices(Devices& devices)
{
    std::pair<Devices::iterator, bool> insertPair = devices.insert(boost::make_shared<Device>(1));
    if (insertPair.second && insertPair.first != devices.end())
    {
        devices.modify(insertPair.first, Device::update_ip_address("192.168.1.10"));
        (*insertPair.first)->deviceType("hc800");
    }

    insertPair = devices.insert(boost::make_shared<Device>(2));
    if (insertPair.second && insertPair.first != devices.end())
    {
        devices.modify(insertPair.first, Device::update_ip_address("192.168.1.9"));
        (*insertPair.first)->deviceType("hc250");
    }

    insertPair = devices.insert(boost::make_shared<Device>(3));
    if (insertPair.second && insertPair.first != devices.end())
    {
        devices.modify(insertPair.first, Device::update_ip_address("192.168.1.8"));
        (*insertPair.first)->deviceType("c7");
    }

    std::cout << "indexed by device id" << std::endl;
    BOOST_FOREACH(const Device::Ptr device, devices)
    {
        printDevice(device);
    }

    Devices::index<Device::by_address>::type::iterator addrIt = devices.get<Device::by_address>().find("192.168.1.9");
    if (addrIt != devices.get<Device::by_address>().end())
    {
        devices.get<Device::by_address>().modify(addrIt, Device::update_ip_address("192.168.1.20"));
        std::cout << std::endl;
        std::cout << "indexed by device id (after modification)" << std::endl;
        BOOST_FOREACH(const Device::Ptr device, devices)
        {
            printDevice(device);
        }
    }
    else
    {
        std::cout << "did not find" << std::endl;
    }
}

void createDeviceTypes(DeviceTypes& deviceTypes)
{
    deviceTypes.insert(boost::make_shared<DeviceType>("hc800", "2.8.0.987654"));
    deviceTypes.insert(boost::make_shared<DeviceType>("hc250", "2.8.0.987654"));
    deviceTypes.insert(boost::make_shared<DeviceType>("c7", "2.8.0.987654"));

    std::cout << "indexed by name" << std::endl;
    BOOST_FOREACH(const DeviceType::Ptr deviceType, deviceTypes)
    {
        printDeviceType(deviceType);
    }
}

void createPackages(Packages& packages)
{
    packages.insert(boost::make_shared<Package>("preupgrade_2.8.0.987654-res_all.deb",
        "preupgrade",
        4144,
        "801cb66b56c3a2ae4bca59f9aed338cb",
        ""));

    packages.insert(boost::make_shared<Package>("upman-server_2.8.0.987654-res_i386v2.deb",
        "upman-server",
        463462,
        "2c1520ad68b0a3d49d1c8234e9bc03ca",
        ""));

    packages.insert(boost::make_shared<Package>("c4drivers_2.8.0.987654-res_i386v2.deb",
        "c4drivers",
        14147492,
        "4bc99ac05a93fb6bac38e1a593e2c81c",
        ""));

    packages.insert(boost::make_shared<Package>("c4drivers_2.8.0.987654-res_armv7av2.deb",
        "c4drivers",
        16163682,
        "4099ce73f6ca1e068e576f72a1e3d21e",
        ""));

    packages.insert(boost::make_shared<Package>("system-version_2.8.0.987654-res_all.deb",
        "system-version",
        798,
        "89f58710d06ac3f5ca32ad61a3a796c8",
        ""));

    std::cout << "indexed by filename" << std::endl;
    BOOST_FOREACH(const Package::Ptr package, packages)
    {
        printPackage(package);
    }
}

int main(int argc, char* argv[])
{
    Devices devices;
    createDevices(devices);

    std::cout << std::endl;

    DeviceTypes deviceTypes;
    createDeviceTypes(deviceTypes);

    std::cout << std::endl;

    Packages packages;
    createPackages(packages);



    std::cout << std::endl;
    std::cout << "finished" << std::endl;

    return 0;
}
