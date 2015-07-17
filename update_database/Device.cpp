/*
 * Device.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: ryan
 */

#include <Device.hpp>

boost::uint32_t Device::nextBindingId_ = 9299;

Device::Device(boost::uint32_t deviceId) :
    deviceId_(deviceId),
    bindingId_(++nextBindingId_)
{
}

Device::~Device()
{
}

void Device::ipAddress(const std::string& ipAddress)
{
    ipAddress_ = ipAddress;
}

void Device::deviceType(const std::string& deviceType)
{
    deviceType_ = deviceType;
}
