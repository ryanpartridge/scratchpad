/*
 * coproto_service.hpp
 *
 *  Created on: Aug 19, 2016
 *      Author: rpartridge
 */

#ifndef COPROTO_SERVICE_HPP_
#define COPROTO_SERVICE_HPP_

#include <boost/asio/io_service.hpp>

class coproto_service :
    public boost::asio::detail::service_base<coproto_service>
{
public:
    explicit coproto_service(boost::asio::io_service& io_service) :
        boost::asio::detail::service_base<coproto_service>(io_service)
    {
    }
};

#endif /* COPROTO_SERVICE_HPP_ */
