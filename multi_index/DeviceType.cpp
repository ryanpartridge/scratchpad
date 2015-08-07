/*
 * DeviceType.cpp
 *
 *  Created on: Aug 7, 2015
 *      Author: rpartridge
 */

#include <DeviceType.hpp>

DeviceType::DeviceType(bool markedForDownload, double ratio) :
    markedForDownload_(markedForDownload),
    ratio_(ratio)
{
}

DeviceType::~DeviceType()
{
}

