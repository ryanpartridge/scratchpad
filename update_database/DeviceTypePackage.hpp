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
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <Package.hpp>

class DeviceTypePackage :
    public boost::enable_shared_from_this<DeviceTypePackage>
{
public:
    typedef boost::shared_ptr<DeviceTypePackage> Ptr;

    DeviceTypePackage(const std::string& deviceType, Package::Ptr package);
    virtual ~DeviceTypePackage();

    const std::string& deviceType() const { return deviceType_; }
    const Package::Ptr package() const { return package_; }
    bool downloaded() const { return downloaded_; }

private:
    void downloaded(bool downloaded);

    std::string deviceType_;
    Package::Ptr package_;
    bool downloaded_;

public:
    // for use with multi-index
    struct by_downloaded{};

    // for use with multi-index modify
    struct set_downloaded
    {
        set_downloaded(bool downloaded) :
            downloaded_(downloaded)
        {
        }

        void operator()(DeviceTypePackage& package)
        {
            package.downloaded(downloaded_);
        }

        void operator()(DeviceTypePackage::Ptr package)
        {
            package->downloaded(downloaded_);
        }

    private:
        bool downloaded_;
    };
};

typedef boost::multi_index_container<
    DeviceTypePackage::Ptr,
    boost::multi_index::indexed_by<
        boost::multi_index::sequenced<>,
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<DeviceTypePackage::by_downloaded>,
            boost::multi_index::const_mem_fun<DeviceTypePackage, bool, &DeviceTypePackage::downloaded>
        >
    >
> DeviceTypePackages;

#endif /* DEVICETYPEPACKAGE_HPP_ */
