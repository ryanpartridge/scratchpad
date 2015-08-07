/*
 * DeviceType.hpp
 *
 *  Created on: Aug 7, 2015
 *      Author: rpartridge
 */

#ifndef DEVICETYPE_HPP_
#define DEVICETYPE_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/mem_fun.hpp>

class DeviceType :
    public boost::enable_shared_from_this<DeviceType>,
    private boost::noncopyable
{
public:
    typedef boost::shared_ptr<DeviceType> Ptr;
    DeviceType(bool markedForDownload, double ratio);
    virtual ~DeviceType();

    bool markedForDownload() const { return markedForDownload_; }
    double ratio() const { return ratio_; }

    struct modify_mark
    {
        modify_mark(bool mark)
        {
            mark_ = mark;
        }

        void operator()(Ptr devType)
        {
            devType->markedForDownload_ = mark_;
        }

    private:
        bool mark_;
    };

private:
    bool markedForDownload_;
    double ratio_;
};

typedef boost::multi_index_container<
    DeviceType::Ptr,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_non_unique<
            boost::multi_index::composite_key<
                DeviceType,
                boost::multi_index::const_mem_fun<DeviceType, bool, &DeviceType::markedForDownload>,
                boost::multi_index::const_mem_fun<DeviceType, double, &DeviceType::ratio>
            >
        >
    >
> DeviceTypes;


#endif /* DEVICETYPE_HPP_ */
