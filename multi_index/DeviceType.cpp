/*
 * DeviceType.cpp
 *
 *  Created on: Aug 7, 2015
 *      Author: rpartridge
 */

#include <DeviceType.hpp>

DeviceType::DeviceType(const std::string& name, bool markedForDownload, double ratio) :
    name_(name),
    markedForDownload_(markedForDownload),
    ratio_(ratio)
{
}

DeviceType::~DeviceType()
{
}

