/*
 * DeviceType.cpp
 *
 *  Created on: Jul 15, 2015
 *      Author: rpartridge
 */

#include <DeviceType.hpp>

DeviceType::DeviceType(const std::string& name, const std::string& platform, const std::string& targetVersion) :
    name_(name),
    platform_(platform),
    targetVersion_(targetVersion)
{
}

DeviceType::~DeviceType()
{
}

