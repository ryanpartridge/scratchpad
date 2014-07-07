/*
 * DeviceUpdateInfo.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: ryan
 */

#include "DeviceUpdateInfo.h"

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
