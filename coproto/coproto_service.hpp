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

class coproto_service :
    public boost::asio::detail::service_base<coproto_service>
{
public:
    struct implementation_type :
        private boost::asio::detail::noncopyable
    {
        int value;
        boost::shared_ptr<boost::asio::steady_timer> timer;

        ~implementation_type()
        {
            std::cout << "destroying implementation_type" << std::endl;
        }
    };

    explicit coproto_service(boost::asio::io_service& io_service) :
        boost::asio::detail::service_base<coproto_service>(io_service)
    {
    }

    virtual ~coproto_service()
    {
        std::cout << "destroying coproto service" << std::endl;
    }

    void construct(implementation_type& impl)
    {
        impl.value = 1;
    }

    void destroy(implementation_type& impl)
    {
        impl.value = 0;
    }

    template <typename do_handler>
    BOOST_ASIO_INITFN_RESULT_TYPE(do_handler,
        void (boost::system::error_code))
    async_do(implementation_type& impl,
        BOOST_ASIO_MOVE_ARG(do_handler) handler)
    {
        boost::asio::detail::async_result_init<
            do_handler, void (boost::system::error_code)> init(
                BOOST_ASIO_MOVE_CAST(do_handler)(handler));

        // do something cool
        std::cout << "***async operation here***" << std::endl;
        impl.timer = boost::make_shared<boost::asio::steady_timer>(
            boost::ref(get_io_service()),
            boost::chrono::seconds(2));
        impl.timer->async_wait(boost::bind(
            &coproto_service::handleTimer,
            this,
            boost::asio::placeholders::error));

        return init.result.get();
    }

private:
    void handleTimer(const boost::system::error_code& ec)
    {
        std::cout << "***timer callback***" << std::endl;
        if (ec)
        {
            std::cout << "timer expired with error: " << ec.message() << std::endl;
        }
    }

    void shutdown_service()
    {
        std::cout << "shutting down coproto service" << std::endl;
    }
};

#endif /* COPROTO_SERVICE_HPP_ */
