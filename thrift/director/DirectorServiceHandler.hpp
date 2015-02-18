/*
 * DirectorServiceHandler.hpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */

#ifndef DIRECTORSERVICEHANDLER_HPP_
#define DIRECTORSERVICEHANDLER_HPP_

#include <boost/shared_ptr.hpp>
#include <DirectorService.h>
#include <DriverClient.hpp>

namespace c4
{
    class DirectorServiceHandler : virtual public DirectorServiceIf
    {
    public:
        DirectorServiceHandler(const std::string& clientPath);
        virtual ~DirectorServiceHandler();

        void getValue(std::string& _return, const int32_t id);

    private:
        std::string clientPath_;
        boost::shared_ptr<c4::DriverClient> client_;
    };

} /* namespace c4 */

#endif /* DIRECTORSERVICEHANDLER_HPP_ */
