/*
 * DriverServiceHandler.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */

#include <boost/make_shared.hpp>
#include <DriverServiceHandler.hpp>

namespace c4
{

    DriverServiceHandler::DriverServiceHandler(const std::string& clientPath) :
        client_(boost::make_shared<c4::DirectorClient>(clientPath))
    {
    }

    DriverServiceHandler::~DriverServiceHandler()
    {
        client_.reset();
    }

    void DriverServiceHandler::doAction(std::string& _return, const int32_t deviceId)
    {
        if (deviceId > 0)
        {
            client_->initialize();
            _return += client_->getValue(deviceId - 1);
        }
        _return += " AND driver_daemon answer";
    }

} /* namespace c4 */
