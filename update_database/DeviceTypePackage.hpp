/*
 * DeviceTypePackage.hpp
 *
 *  Created on: Jul 16, 2015
 *      Author: rpartridge
 */

#ifndef DEVICETYPEPACKAGE_HPP_
#define DEVICETYPEPACKAGE_HPP_

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <Package.hpp>

class DeviceTypePackage :
    public boost::enable_shared_from_this<DeviceTypePackage>
{
public:
    DeviceTypePackage(const std::string& deviceType, Package::Ptr package);
    virtual ~DeviceTypePackage();

private:
    std::string deviceType_;
    Package::Ptr package_;
    bool downloaded_;
};

#endif /* DEVICETYPEPACKAGE_HPP_ */
