/*
 * DriverServiceHandler.hpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */

#ifndef DRIVERSERVICEHANDLER_HPP_
#define DRIVERSERVICEHANDLER_HPP_

#include <boost/shared_ptr.hpp>
#include <DriverService.h>
#include <DirectorClient.hpp>

namespace c4
{
    class DriverServiceHandler : virtual public DriverServiceIf
    {
    public:
        DriverServiceHandler(const std::string& clientPath);
        virtual ~DriverServiceHandler();

        void doAction(std::string& _return, const int32_t driverId);

    private:
        std::string clientPath_;
        boost::shared_ptr<c4::DirectorClient> client_;
    };
} /* namespace c4 */

#endif /* DRIVERSERVICEHANDLER_HPP_ */
