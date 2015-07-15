/*
 * Package.hpp
 *
 *  Created on: Jul 15, 2015
 *      Author: rpartridge
 */

#ifndef PACKAGE_HPP_
#define PACKAGE_HPP_

#include <string>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/mem_fun.hpp>

class Package
{
public:
    typedef boost::shared_ptr<Package> Ptr;

    Package();
    virtual ~Package();

    const std::string& filename() const { return filename_; }
    const std::string& name() const { return name_; }
    boost::uint32_t size() const { return size_; }
    const std::string& checksum() const { return checksum_; }
    const std::string& location() const { return location_; }
    const std::string& platform() const { return platform_; }

private:
    std::string filename_;
    std::string name_;
    boost::uint32_t size_;
    std::string checksum_;
    std::string location_;
    std::string platform_;

public:
};

#endif /* PACKAGE_HPP_ */
