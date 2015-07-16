/*
 * DevicePackage.hpp
 *
 *  Created on: Jul 16, 2015
 *      Author: rpartridge
 */

#ifndef DEVICEPACKAGE_HPP_
#define DEVICEPACKAGE_HPP_

#include <string>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <Package.hpp>

class DevicePackage :
    public boost::enable_shared_from_this<DevicePackage>
{
public:
    DevicePackage(boost::uint32_t device, Package::Ptr package);
    virtual ~DevicePackage();

private:
    boost::uint32_t device_;
    Package::Ptr package_;
    bool downloaded_;
    bool updated_;
};

#endif /* DEVICEPACKAGE_HPP_ */
