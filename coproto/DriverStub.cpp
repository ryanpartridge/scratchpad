/*
 * DriverStub.cpp
 *
 *  Created on: Sep 20, 2016
 *      Author: rpartridge
 */

#include <boost/lexical_cast.hpp>

#include <DriverStub.hpp>
#include <coproto_handle.hpp>

DriverStub::DriverStub(boost::asio::io_service& io_service) :
    io_service_(io_service)
{
}

DriverStub::~DriverStub()
{
}

std::size_t DriverStub::getCount()
{
    return 0;
}

std::size_t DriverStub::getCount(boost::asio::yield_context yield)
{
    boost::system::error_code ec;
    std::ostringstream req;
    req << "getCount";
    coproto_handle handle(io_service_, req.str());
    std::size_t response = boost::lexical_cast<std::size_t>(handle.async_do(yield[ec]));
    return response;
}
