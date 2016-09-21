/*
 * DirectorStub.cpp
 *
 *  Created on: Sep 20, 2016
 *      Author: rpartridge
 */

#include <sstream>
#include <boost/algorithm/string.hpp>

#include <DirectorStub.hpp>
#include <coproto_handle.hpp>

DirectorStub::DirectorStub(boost::asio::io_service& io_service) :
    io_service_(io_service)
{

}

DirectorStub::~DirectorStub()
{
}

std::string DirectorStub::getValue(const std::string& name, boost::asio::yield_context yield)
{
    boost::system::error_code ec;
    std::ostringstream req;
    req << "getValue|" << name;
    coproto_handle handle(io_service_, req.str());
    std::string response = handle.async_do(yield[ec]);
    return response;
}

bool DirectorStub::isSet(const std::string& name, boost::asio::yield_context yield)
{
    boost::system::error_code ec;
    std::ostringstream req;
    req << "isSet|" << name;
    coproto_handle handle(io_service_, req.str());
    return boost::algorithm::iequals(handle.async_do(yield[ec]), "true");
}
