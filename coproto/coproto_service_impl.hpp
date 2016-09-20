/*
 * coproto_service_impl.hpp
 *
 *  Created on: Sep 8, 2016
 *      Author: rpartridge
 */

#ifndef COPROTO_SERVICE_IMPL_HPP_
#define COPROTO_SERVICE_IMPL_HPP_

#include <iostream>
#include <sstream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/detail/addressof.hpp>
#include <boost/asio/detail/handler_alloc_helpers.hpp>
#include <boost/asio/detail/op_queue.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>

#include <coproto_handler.hpp>
#include <coproto_op.hpp>

template <typename DataType, template <typename> class QueueOwner>
class coproto_service_impl
{
public:
    class implementation_type
    {
    public:
        implementation_type() :
            requestId_(++requestCount_)
        {
            std::cout << "implementation_type constructor" << std::endl;
        };

        ~implementation_type()
        {
            std::cout << "implementation_type destructor" << std::endl;
        }

    private:
        friend class coproto_service_impl;

        std::string request_;
        std::size_t requestId_;
        static std::size_t requestCount_;
    };

    coproto_service_impl(boost::asio::io_service& io_service) :
        io_service_(boost::asio::use_service<boost::asio::detail::io_service_impl>(io_service)),
        in_queue_(QueueOwner<DataType>::get_in_queue()),
        out_queue_(QueueOwner<DataType>::get_out_queue())
    {
        // add_service(this);
        // TODO: decide if this is needed, and if the
        // pipes in the reactor are also needed
        std::cout << "coproto_service_impl constructor" << std::endl;
    }

    virtual ~coproto_service_impl()
    {
        std::cout << "coproto_service_impl destructor" << std::endl;
    }

    void construct(implementation_type& impl)
    {
        std::cout << "calling construct on coproto_service_impl" << std::endl;
        impl.request_.clear();
    }

    void destroy(implementation_type& impl)
    {
        std::cout << "calling destroy on coproto_service_impl" << std::endl;
        boost::system::error_code ignored_ec;

        // empty the queue_ the right way
        // clear(impl, ignored_ec);
        // cancel(impl, ignroed_ec);
    }

    std::string request(implementation_type& impl) const
    {
        return impl.request_;
    }

    void request(implementation_type& impl, const std::string& req)
    {
        impl.request_ = req;
    }

    template <typename Handler>
    void async_do(implementation_type& impl, Handler& handler)
    {
        typedef coproto_handler<Handler> op;
        typename op::ptr p = { boost::asio::detail::addressof(handler),
            boost_asio_handler_alloc_helpers::allocate(
                sizeof(op), handler), 0 };
        p.p = new (p.v) op(handler);

        BOOST_ASIO_HANDLER_CREATION((p.p, "coproto_handle", &impl, "async_do"));

        start_do_op(impl, p.p);
        p.v = p.p = 0;
    }

    void shutdown_service()
    {
        std::cout << "shutting down coproto_service_impl" << std::endl;
        // TODO: figure out shutdown order
        //io_service_.abandon_operations(ops)
    }

private:
    void start_do_op(implementation_type& impl, coproto_op* op)
    {
        io_service_.work_started();

        std::ostringstream request;
        request << "req|" << impl.requestId_ << "|" << impl.request_ << "\r\n";
        out_queue_.push(request.str());

        // TODO: something here
        timer_ = boost::make_shared<boost::asio::steady_timer>(
            boost::ref(io_service_.get_io_service()),
            boost::chrono::seconds(5));
        timer_->async_wait(boost::bind(
            &coproto_service_impl::handleTimer,
            this,
            boost::asio::placeholders::error));

        op_queue_.push(op);
    }

    void handleTimer(const boost::system::error_code& ec)
    {
        std::cout << "***timer callback***" << std::endl;
        if (timer_)
        {
            timer_->cancel();
            timer_.reset();
        }
        coproto_op* op = op_queue_.front();
        if (op)
        {
            if (ec)
            {
                std::cout << "timer expired with error: " << ec.message() << std::endl;
                op->ec_ = ec;
            }
            op->value_ = "Hello ASIO!";
            op_queue_.pop();
            io_service_.post_deferred_completion(op);
        }
    }

    typedef typename QueueOwner<DataType>::queue_type queue_type;

    boost::asio::detail::io_service_impl& io_service_;
    boost::shared_ptr<boost::asio::steady_timer> timer_;
    boost::asio::detail::op_queue<coproto_op> op_queue_;
    queue_type& in_queue_;
    queue_type& out_queue_;

};

template <typename D, template <typename> class Q> std::size_t coproto_service_impl<D, Q>::implementation_type::requestCount_ = 0;

#endif /* COPROTO_SERVICE_IMPL_HPP_ */
