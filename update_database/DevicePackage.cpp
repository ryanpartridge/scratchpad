/*
 * DevicePackage.cpp
 *
 *  Created on: Jul 16, 2015
 *      Author: rpartridge
 */

#include <DevicePackage.hpp>

DevicePackage::DevicePackage(boost::uint32_t deviceId, Package::Ptr package) :
    deviceId_(deviceId),
    package_(package),
    downloaded_(false),
    updated_(false)
{
}

DevicePackage::~DevicePackage()
{
}

void DevicePackage::downloaded(bool downloaded)
{
    downloaded_ = downloaded;
}

void DevicePackage::updated(bool updated)
{
    updated_ = updated;
}
