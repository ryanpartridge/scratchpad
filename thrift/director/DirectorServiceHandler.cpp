/*
 * DirectorServiceHandler.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */

#include <boost/make_shared.hpp>
#include <DirectorServiceHandler.hpp>

namespace c4
{

    DirectorServiceHandler::DirectorServiceHandler(const std::string& clientPath) :
        client_(boost::make_shared<c4::DriverClient>(clientPath))
    {
    }

    DirectorServiceHandler::~DirectorServiceHandler()
    {
        client_.reset();
    }

    void DirectorServiceHandler::getValue(std::string& _return, const int32_t id)
    {
        if (id > 0)
        {
            client_->initialize();
            _return += client_->doAction(id - 1);
        }
        _return += " AND director answer";
    }

} /* namespace c4 */
