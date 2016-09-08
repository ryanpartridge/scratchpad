/*
 * coproto_handle.hpp
 *
 *  Created on: Aug 22, 2016
 *      Author: rpartridge
 */

#ifndef COPROTO_HANDLE_HPP_
#define COPROTO_HANDLE_HPP_

#include <iostream>
#include <boost/asio/basic_io_object.hpp>

#include <coproto_service.hpp>

class coproto_handle :
    public boost::asio::basic_io_object<coproto_service>
{
public:
    explicit coproto_handle(boost::asio::io_service& io_service) :
        boost::asio::basic_io_object<coproto_service>(io_service)
    {

    }

    virtual ~coproto_handle()
    {
        std::cout << "destroying coproto_handle" << std::endl;
    }

    template <typename DoHandler>
    BOOST_ASIO_INITFN_RESULT_TYPE(DoHandler,
        void (boost::system::error_code))
    async_do(BOOST_ASIO_MOVE_ARG(DoHandler) handler)
    {
        // HANDLER_CHECK macro needs to be created and called

        return this->service.async_do(this->implementation,
            BOOST_ASIO_MOVE_CAST(DoHandler)(handler));
    }
};

#endif /* COPROTO_HANDLE_HPP_ */
