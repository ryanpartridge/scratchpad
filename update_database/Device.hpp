/*
 * Device.hpp
 *
 *  Created on: Jul 3, 2014
 *      Author: ryan
 */

#ifndef DEVICE_HPP_
#define DEVICE_HPP_

#include <string>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <DeviceType.hpp>

class Device :
    public boost::enable_shared_from_this<Device>
{
public:
    typedef boost::shared_ptr<Device> Ptr;

    explicit Device(unsigned int deviceId);
    virtual ~Device();

    boost::uint32_t deviceId() const { return deviceId_; }
    const std::string& ipAddress() const { return ipAddress_; }
    boost::uint32_t bindingId() const { return bindingId_; }
    DeviceType::Ptr deviceType() const { return deviceType_; }

private:
    void ipAddress(const std::string& ipAddress);

    boost::uint32_t deviceId_;
    std::string ipAddress_;
    boost::uint32_t bindingId_;
    DeviceType::Ptr deviceType_;

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

        void operator()(Device& dui)
        {
            dui.ipAddress(ipAddress_);
        }

        void operator()(Device::Ptr dui)
        {
            dui->ipAddress(ipAddress_);
        }

    private:
        std::string ipAddress_;
    };
};

typedef boost::multi_index_container<
    Device::Ptr,
    boost::multi_index::indexed_by<
        boost::multi_index::hashed_unique<
            boost::multi_index::tag<Device::by_device>,
            boost::multi_index::const_mem_fun<Device, boost::uint32_t, &Device::deviceId>
        >,
        boost::multi_index::hashed_non_unique<
            boost::multi_index::tag<Device::by_address>,
            boost::multi_index::const_mem_fun<Device, const std::string&, &Device::ipAddress>
        >,
        boost::multi_index::hashed_unique<
            boost::multi_index::tag<Device::by_binding>,
            boost::multi_index::const_mem_fun<Device, boost::uint32_t, &Device::bindingId>
        >
    >
> Devices;

#endif /* DEVICE_HPP_ */
