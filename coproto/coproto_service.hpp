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

class coproto_service :
    public boost::asio::detail::service_base<coproto_service>
{
public:
    typedef coproto_service_impl::implementation_type implementation_type;

    explicit coproto_service(boost::asio::io_service& io_service) :
        boost::asio::detail::service_base<coproto_service>(io_service),
        service_impl_(io_service)
    {
    }

    virtual ~coproto_service()
    {
        std::cout << "destroying coproto service" << std::endl;
    }

    void construct(implementation_type& impl)
    {
    }

    void destroy(implementation_type& impl)
    {
    }

    template <typename DoHandler>
    BOOST_ASIO_INITFN_RESULT_TYPE(DoHandler,
        void (boost::system::error_code))
    async_do(implementation_type& impl,
        BOOST_ASIO_MOVE_ARG(DoHandler) handler)
    {
        boost::asio::detail::async_result_init<
            DoHandler, void (boost::system::error_code)> init(
                BOOST_ASIO_MOVE_CAST(DoHandler)(handler));

        // do something cool
        std::cout << "***async operation here***" << std::endl;
        timer_ = boost::make_shared<boost::asio::steady_timer>(
            boost::ref(get_io_service()),
            boost::chrono::seconds(2));
        timer_->async_wait(boost::bind(
            &coproto_service::handleTimer,
            this,
            boost::asio::placeholders::error));
        std::cout << "***async operation queued***" << std::endl;

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
        timer_->cancel();
        timer_.reset();
        std::cout << "shutting down coproto service" << std::endl;
    }

    void fork_service(boost::asio::io_service::fork_event /* event */)
    {
        // TODO: need to implement this
    }

    coproto_service_impl service_impl_;
    boost::shared_ptr<boost::asio::steady_timer> timer_;
};

#endif /* COPROTO_SERVICE_HPP_ */
