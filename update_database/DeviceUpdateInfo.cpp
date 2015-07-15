/*
 * DeviceUpdateInfo.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: ryan
 */

#include <DeviceUpdateInfo.hpp>

boost::uint32_t DeviceUpdateInfo::nextBindingId_ = 9299;

DeviceUpdateInfo::DeviceUpdateInfo(boost::uint32_t deviceId) :
    deviceId_(deviceId),
    bindingId_(++nextBindingId_)
{
}

DeviceUpdateInfo::~DeviceUpdateInfo()
{
}

void DeviceUpdateInfo::ipAddress(const std::string& ipAddress)
{
    ipAddress_ = ipAddress;
}
