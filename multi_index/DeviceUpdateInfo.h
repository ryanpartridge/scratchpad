/*
 * DeviceUpdateInfo.h
 *
 *  Created on: Jul 3, 2014
 *      Author: ryan
 */

#ifndef DEVICEUPDATEINFO_H_
#define DEVICEUPDATEINFO_H_

#include <string>
#include <boost/cstdint.hpp>

class DeviceUpdateInfo
{
public:
    DeviceUpdateInfo();
    DeviceUpdateInfo(unsigned int deviceId_);
    virtual ~DeviceUpdateInfo();

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

typedef boost::multi_index_container<
    DeviceUpdateInfo,
    boost::multi_index::indexed_by<
        boost::multi_index::hashed_unique<boost::multi_index::identity<DeviceUpdateInfo> >,
        boost::multi_index::hashed_unique<boost::multi_index::member<DeviceUpdateInfo, const std::string, &DeviceUpdateInfo::ipAddress_> >,
        boost::multi_index::hashed_unique<boost::multi_index::member<StateEntry, const boost::uint32_t, &DeviceUpdateInfo::bindingId_> >
    >
> DeviceUpdateInfoSet;

#endif /* DEVICEUPDATEINFO_H_ */
