/*
 * DeviceType.hpp
 *
 *  Created on: Jul 15, 2015
 *      Author: rpartridge
 */

#ifndef DEVICETYPE_HPP_
#define DEVICETYPE_HPP_

#include <string>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <DeviceTypePackage.hpp>

class DeviceType :
    public boost::enable_shared_from_this<DeviceType>
{
public:
    typedef boost::shared_ptr<DeviceType> Ptr;

    DeviceType(const std::string& name, const std::string& targetVersion);
    virtual ~DeviceType();

    const std::string& name() const { return name_; }
    const std::string& targetVersion() const { return targetVersion_; }

    void clearPackages();
    void addPackage(Package::Ptr package);

private:
    std::string name_;
    std::string targetVersion_;
    DeviceTypePackages packages_;

public:
    // for multi-index tags
    struct by_name{};
    struct by_targetVersion{};
};

typedef boost::multi_index_container<
    DeviceType::Ptr,
    boost::multi_index::indexed_by<
        boost::multi_index::hashed_unique<
            boost::multi_index::tag<DeviceType::by_name>,
            boost::multi_index::const_mem_fun<DeviceType, const std::string&, &DeviceType::name>
        >,
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<DeviceType::by_targetVersion>,
            boost::multi_index::const_mem_fun<DeviceType, const std::string&, &DeviceType::targetVersion>
        >
    >
> DeviceTypes;

#endif /* DEVICETYPE_HPP_ */
