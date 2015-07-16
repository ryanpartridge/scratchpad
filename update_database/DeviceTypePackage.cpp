/*
 * DeviceTypePackage.cpp
 *
 *  Created on: Jul 16, 2015
 *      Author: rpartridge
 */

#include <DeviceTypePackage.hpp>

DeviceTypePackage::DeviceTypePackage(const std::string& deviceType, Package::Ptr package) :
    deviceType_(deviceType),
    package_(package),
    downloaded_(false)
{
}

DeviceTypePackage::~DeviceTypePackage()
{
}

