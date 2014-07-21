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


class DeviceUpdateInfo
{
public:
    DeviceUpdateInfo();
    DeviceUpdateInfo(unsigned int deviceId_);
    virtual ~DeviceUpdateInfo();

    const boost::uint32_t deviceId() const { return deviceId_; }
    const std::string& ipAddress() const { return ipAddress_; }
    const boost::uint32_t bindingId() const { return bindingId_; }

    void deviceId(const boost::uint32_t deviceId);
    void ipAddress(const std::string& ipAddress);
    void bindingId(const boost::uint32_t bindingId);

    bool operator<(const DeviceUpdateInfo& dui) const
    {
        return deviceId_ < dui.deviceId_;
    }

    bool operator==(const DeviceUpdateInfo& dui) const
    {
        return deviceId_ == dui.deviceId_;
    }

private:
    boost::uint32_t deviceId_;
    std::string ipAddress_;
    boost::uint32_t bindingId_;
};

typedef boost::shared_ptr<DeviceUpdateInfo> SP_DeviceUpdateInfo;

struct device_id_key_extractor
{
    typedef boost::uint32_t result_type;
    result_type operator()(const DeviceUpdateInfo& dui) const { return dui.deviceId(); }
};

struct ip_address_key_extractor
{
    typedef std::string result_type;
    result_type operator()(const DeviceUpdateInfo& dui) const { return dui.ipAddress(); }
};

struct binding_id_key_extractor
{
    typedef boost::uint32_t result_type;
    result_type operator()(const DeviceUpdateInfo& dui) const { return dui.bindingId(); }
};

struct sp_device_id_key_extractor
{
	typedef boost::uint32_t result_type;
	result_type operator()(SP_DeviceUpdateInfo dui) const { return dui->deviceId(); }
};

struct sp_ip_address_key_extractor
{
	typedef std::string result_type;
	result_type operator()(SP_DeviceUpdateInfo dui) const { return dui->ipAddress(); }
};

struct sp_binding_id_key_extractor
{
	typedef boost::uint32_t result_type;
	result_type operator()(SP_DeviceUpdateInfo dui) const { return dui->bindingId(); }
};

struct by_device{};
struct by_address{};
struct by_binding{};


typedef boost::multi_index_container<
    DeviceUpdateInfo,
    boost::multi_index::indexed_by<
        boost::multi_index::hashed_unique<boost::multi_index::tag<by_device>, device_id_key_extractor>,
        boost::multi_index::hashed_non_unique<boost::multi_index::tag<by_address>, ip_address_key_extractor>,
        boost::multi_index::hashed_non_unique<boost::multi_index::tag<by_binding>, binding_id_key_extractor>
    >
> DeviceUpdateInfoSet;

typedef boost::multi_index_container<
    SP_DeviceUpdateInfo,
    boost::multi_index::indexed_by<
        boost::multi_index::hashed_unique<boost::multi_index::tag<by_device>, sp_device_id_key_extractor>,
        boost::multi_index::hashed_non_unique<boost::multi_index::tag<by_address>, sp_ip_address_key_extractor>,
        boost::multi_index::hashed_non_unique<boost::multi_index::tag<by_binding>, sp_binding_id_key_extractor>
    >
> SP_DeviceUpdateInfoSet;

#endif /* DEVICEUPDATEINFO_HPP_ */
