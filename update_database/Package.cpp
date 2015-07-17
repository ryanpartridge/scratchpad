/*
 * Package.cpp
 *
 *  Created on: Jul 15, 2015
 *      Author: rpartridge
 */

#include <Package.hpp>

Package::Package(const std::string& filename,
    const std::string& name,
    boost::uint32_t size,
    const std::string& checksum,
    const std::string& location) :
    filename_(filename),
    name_(name),
    size_(size),
    checksum_(checksum),
    location_(location)
{
}

Package::~Package()
{
}

