/*
 * coproto_service_impl.hpp
 *
 *  Created on: Sep 8, 2016
 *      Author: rpartridge
 */

#ifndef COPROTO_SERVICE_IMPL_HPP_
#define COPROTO_SERVICE_IMPL_HPP_

#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/detail/op_queue.hpp>

#include <coproto_op.hpp>

class coproto_service_impl
{
public:
    class implementation_type
    {
    public:
        implementation_type() :
            value_(0)
        {
        };

        ~implementation_type()
        {
            std::cout << "destroying implementation_type" << std::endl;
        }

    private:
        friend class coproto_service_impl;

        boost::asio::detail::op_queue<coproto_op> queue_;
        int value_;
    };

    coproto_service_impl(boost::asio::io_service& io_service) :
        io_service_(boost::asio::use_service<boost::asio::detail::io_service_impl>(io_service))
    {
    }

private:
    boost::asio::detail::io_service_impl& io_service_;
};



#endif /* COPROTO_SERVICE_IMPL_HPP_ */
