/*
 * DeviceUpdateInfo.hpp
 *
 *  Created on: Jul 3, 2014
 *      Author: ryan
 */

#ifndef DEVICEUPDATEINFO_HPP_
#define DEVICEUPDATEINFO_HPP_

#include <string>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/mem_fun.hpp>

class DeviceUpdateInfo
{
public:
    typedef boost::shared_ptr<DeviceUpdateInfo> Ptr;

    DeviceUpdateInfo(unsigned int deviceId_);
    virtual ~DeviceUpdateInfo();

    boost::uint32_t deviceId() const { return deviceId_; }
    const std::string& ipAddress() const { return ipAddress_; }
    boost::uint32_t bindingId() const { return bindingId_; }

/*
    bool operator<(const DeviceUpdateInfo& dui) const
    {
        return deviceId_ < dui.deviceId_;
    }

    bool operator==(const DeviceUpdateInfo& dui) const
    {
        return deviceId_ == dui.deviceId_;
    }
*/
private:
    void ipAddress(const std::string& ipAddress);

    boost::uint32_t deviceId_;
    std::string ipAddress_;
    boost::uint32_t bindingId_;

    static boost::uint32_t nextBindingId_;

public:
    // for multi-index tags
    struct by_device{};
    struct by_address{};
    struct by_binding{};

    // for use with multi-index modify
    struct update_ip_address
    {
        update_ip_address(const std::string& ipAddress) :
            ipAddress_(ipAddress)
        {
        }

        void operator()(DeviceUpdateInfo& dui)
        {
            dui.ipAddress(ipAddress_);
        }

        void operator()(DeviceUpdateInfo::Ptr dui)
        {
            dui->ipAddress(ipAddress_);
        }

    private:
        std::string ipAddress_;
    };
};

typedef boost::multi_index_container<
    DeviceUpdateInfo::Ptr,
    boost::multi_index::indexed_by<
        boost::multi_index::hashed_unique<
            boost::multi_index::tag<DeviceUpdateInfo::by_device>,
            boost::multi_index::const_mem_fun<DeviceUpdateInfo, boost::uint32_t, &DeviceUpdateInfo::deviceId>
        >,
        boost::multi_index::hashed_non_unique<
            boost::multi_index::tag<DeviceUpdateInfo::by_address>,
            boost::multi_index::const_mem_fun<DeviceUpdateInfo, const std::string&, &DeviceUpdateInfo::ipAddress>
        >,
        boost::multi_index::hashed_unique<
            boost::multi_index::tag<DeviceUpdateInfo::by_binding>,
            boost::multi_index::const_mem_fun<DeviceUpdateInfo, boost::uint32_t, &DeviceUpdateInfo::bindingId>
        >
    >
> DeviceUpdateInfoSet;

#endif /* DEVICEUPDATEINFO_HPP_ */
