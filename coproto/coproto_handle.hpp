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
#include <QueueManager.hpp>

template <typename DataType, template <typename> class QueueOwner>
class basic_coproto_handle :
    public boost::asio::basic_io_object<coproto_service<DataType, QueueOwner> >
{
public:
    explicit basic_coproto_handle(boost::asio::io_service& io_service) :
        boost::asio::basic_io_object<coproto_service<DataType, QueueOwner> >(io_service)
    {
        //std::cout << "coproto_handle constructor" << std::endl;
    }

    basic_coproto_handle(boost::asio::io_service& io_service, const std::string& request) :
        boost::asio::basic_io_object<coproto_service<DataType, QueueOwner> >(io_service)
    {
        //std::cout << "coproto_handle constructor" << std::endl;
        this->service.request(this->implementation, request);
    }

    virtual ~basic_coproto_handle()
    {
        //std::cout << "coproto_handle destructor" << std::endl;
    }

    std::string request() const
    {
        return this->service.request(this->implementation);
    }

    void request(const std::string& req)
    {
        this->service.request(this->implementation, req);
    }

    template <typename DoHandler>
    BOOST_ASIO_INITFN_RESULT_TYPE(DoHandler,
        void (boost::system::error_code, std::string))
    async_do(BOOST_ASIO_MOVE_ARG(DoHandler) handler)
    {
        // HANDLER_CHECK macro needs to be created and called

        return this->service.async_do(this->implementation,
            BOOST_ASIO_MOVE_CAST(DoHandler)(handler));
    }
};

typedef basic_coproto_handle<std::string, QueueManager > coproto_handle;

#endif /* COPROTO_HANDLE_HPP_ */
