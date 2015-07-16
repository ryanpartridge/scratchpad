/*
 * DevicePackage.cpp
 *
 *  Created on: Jul 16, 2015
 *      Author: rpartridge
 */

#include <DevicePackage.hpp>

DevicePackage::DevicePackage(boost::uint32_t device, Package::Ptr package) :
    device_(device),
    package_(package),
    downloaded_(false),
    updated_(false)
{
}

DevicePackage::~DevicePackage()
{
}
