/*
 * coproto_service.hpp
 *
 *  Created on: Aug 19, 2016
 *      Author: rpartridge
 */

#ifndef COPROTO_SERVICE_HPP_
#define COPROTO_SERVICE_HPP_

#include <iostream>

#include <boost/asio/io_service.hpp>
#include <boost/asio/detail/noncopyable.hpp>
#include <boost/asio/async_result.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>
#include <boost/bind.hpp>

#include <coproto_service_impl.hpp>

template <typename DataType, template <typename> class QueueOwner>
class coproto_service :
    public boost::asio::detail::service_base<coproto_service<DataType, QueueOwner> >
{
public:
    typedef coproto_service_impl::implementation_type implementation_type;

    explicit coproto_service(boost::asio::io_service& io_service) :
        boost::asio::detail::service_base<coproto_service<DataType, QueueOwner> >(io_service),
        service_impl_(io_service)
    {
        std::cout << "coproto_service constructor" << std::endl;
    }

    virtual ~coproto_service()
    {
        std::cout << "coproto_service destructor" << std::endl;
    }

    void construct(implementation_type& impl)
    {
        std::cout << "calling construct on coproto_service" << std::endl;
        service_impl_.construct(impl);
    }

    void destroy(implementation_type& impl)
    {
        std::cout << "calling destroy on coproto_service" << std::endl;
        service_impl_.destroy(impl);
    }

    template <typename DoHandler>
    BOOST_ASIO_INITFN_RESULT_TYPE(DoHandler,
        void (boost::system::error_code, int))
    async_do(implementation_type& impl,
        BOOST_ASIO_MOVE_ARG(DoHandler) handler)
    {
        boost::asio::detail::async_result_init<
            DoHandler, void (boost::system::error_code, int)> init(
                BOOST_ASIO_MOVE_CAST(DoHandler)(handler));

        service_impl_.async_do(impl, init.handler);

        return init.result.get();
    }

private:
    void shutdown_service()
    {
        std::cout << "shutting down coproto_service" << std::endl;
        service_impl_.shutdown_service();
    }

    void fork_service(boost::asio::io_service::fork_event /* event */)
    {
        // TODO: need to implement this
    }

    coproto_service_impl service_impl_;
};

#endif /* COPROTO_SERVICE_HPP_ */
