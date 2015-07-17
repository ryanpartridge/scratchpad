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
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <Package.hpp>

class DevicePackage :
    public boost::enable_shared_from_this<DevicePackage>
{
public:
    typedef boost::shared_ptr<DevicePackage> Ptr;

    DevicePackage(boost::uint32_t deviceId, Package::Ptr package);
    virtual ~DevicePackage();

    boost::uint32_t deviceId() const { return deviceId_; }
    const Package::Ptr package() const { return package_; }
    bool downloaded() const { return downloaded_; }
    bool updated() const { return updated_; }

private:
    void downloaded(bool downloaded);
    void updated(bool updated);

    boost::uint32_t deviceId_;
    Package::Ptr package_;
    bool downloaded_;
    bool updated_;

public:
    // for use with multi-index
    struct by_downloaded{};
    struct by_updated{};

    // for use with multi-index modify
    struct set_downloaded
    {
        set_downloaded(bool downloaded) :
            downloaded_(downloaded)
        {
        }

        void operator()(DevicePackage& package)
        {
            package.downloaded(downloaded_);
        }

        void operator()(DevicePackage::Ptr package)
        {
            package->downloaded(downloaded_);
        }

    private:
        bool downloaded_;
    };

    struct set_updated
    {
        set_updated(bool updated) :
            updated_(updated)
        {
        }

        void operator()(DevicePackage& package)
        {
            package.updated(updated_);
        }

        void operator()(DevicePackage::Ptr package)
        {
            package->updated(updated_);
        }

    private:
        bool updated_;
    };
};

typedef boost::multi_index_container<
    DevicePackage::Ptr,
    boost::multi_index::indexed_by<
        boost::multi_index::sequenced<>,
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<DevicePackage::by_downloaded>,
            boost::multi_index::const_mem_fun<DevicePackage, bool, &DevicePackage::downloaded>
        >,
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<DevicePackage::by_updated>,
            boost::multi_index::const_mem_fun<DevicePackage, bool, &DevicePackage::updated>
        >
    >
> DevicePackages;

#endif /* DEVICEPACKAGE_HPP_ */
