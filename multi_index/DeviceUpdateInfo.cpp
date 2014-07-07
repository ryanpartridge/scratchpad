/*
 * DeviceUpdateInfo.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: ryan
 */

#include <DeviceUpdateInfo.hpp>

DeviceUpdateInfo::DeviceUpdateInfo() :
    deviceId_(0),
    ipAddress_(""),
    bindingId_(0)
{
}

DeviceUpdateInfo::DeviceUpdateInfo(boost::uint32_t deviceId) :
    deviceId_(deviceId),
    ipAddress_(""),
    bindingId_(0)
{
}

DeviceUpdateInfo::~DeviceUpdateInfo()
{
}

void DeviceUpdateInfo::deviceId(const boost::uint32_t deviceId)
{
    deviceId_ = deviceId;
}

void DeviceUpdateInfo::ipAddress(const std::string& ipAddress)
{
    ipAddress_ = ipAddress;
}

void DeviceUpdateInfo::bindingId(const boost::uint32_t bindingId)
{
    bindingId_ = bindingId;
}
