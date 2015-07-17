/*
 * DeviceType.cpp
 *
 *  Created on: Jul 15, 2015
 *      Author: rpartridge
 */

#include <boost/make_shared.hpp>

#include <DeviceType.hpp>

DeviceType::DeviceType(const std::string& name, const std::string& targetVersion) :
    name_(name),
    targetVersion_(targetVersion)
{
}

DeviceType::~DeviceType()
{
}

void DeviceType::clearPackages()
{
    packages_.clear();
}

void DeviceType::addPackage(Package::Ptr package)
{
    DeviceTypePackage::Ptr devPackage = boost::make_shared<DeviceTypePackage>(name_, package);
    packages_.push_back(devPackage);
}
