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
#include <vector>
#include <map>
#include <boost/asio/io_service.hpp>
#include <boost/asio/detail/addressof.hpp>
#include <boost/asio/detail/handler_alloc_helpers.hpp>
#include <boost/asio/detail/op_queue.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

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
            request_id_(++request_count_)
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
        std::size_t request_id_;
        static std::size_t request_count_;
    };

    typedef std::map<std::size_t, coproto_op*> op_map;

    coproto_service_impl(boost::asio::io_service& io_service) :
        io_service_(boost::asio::use_service<boost::asio::detail::io_service_impl>(io_service)),
        in_queue_(QueueOwner<DataType>::get_in_queue()),
        out_queue_(QueueOwner<DataType>::get_out_queue()),
        queue_started_(false)
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
        // stop servicing the queue
        //io_service_.abandon_operations(ops)
    }

private:
    void start_do_op(implementation_type& impl, coproto_op* op)
    {
        io_service_.work_started();

        std::ostringstream request;
        request << "req|" << impl.request_id_ << "|" << impl.request_ << "\r\n";
        out_queue_.push(request.str());

        if (!queue_started_)
        {
            // spawn the queue service
            boost::asio::spawn(io_service_.get_io_service(), boost::bind(&coproto_service_impl::service_in_queue, this, _1));
        }

        // store the op away until the response comes in
        op_queue_.push(op);
    }

    void service_in_queue(boost::asio::yield_context yield)
    {
        std::cout << "watching in_queue_ for responses" << std::endl;
        boost::system::error_code ec;
        int in_fd = in_queue_.eventFd();
        if (in_fd == -1)
        {
            std::cout << "error getting the event file descriptor" << std::endl;
            return;
        }

        queue_started_ = true;
        boost::asio::posix::stream_descriptor descriptor(io_service_.get_io_service(), in_fd);
        std::size_t bytes_read = 0;
        std::size_t event_count;
        boost::asio::mutable_buffers_1 buffer(&event_count, sizeof(event_count));
        while ((bytes_read = boost::asio::async_read(descriptor, buffer, yield[ec])) > 0 && !ec)
        {
            std::cout << event_count << " items in the queue" << std::endl;
            std::string payload;
            while (in_queue_.front(payload))
            {
                std::vector<std::string> msgParts;
                boost::algorithm::split(msgParts, payload, boost::algorithm::is_any_of(std::string("|")), boost::algorithm::token_compress_on);
                std::size_t request_id = boost::lexical_cast<boost::uint32_t>(msgParts[1]);
                op_map::iterator it = op_map_.find(request_id);
                if (it != op_map_.end())
                {
                    coproto_op* op = it->second;
                    op->value_ = msgParts[3];
                    io_service_.post_deferred_completion(op);
                }
                in_queue_.pop();
            }
            event_count = 0;
        }
        queue_started_ = false;
    }

    typedef typename QueueOwner<DataType>::queue_type queue_type;

    boost::asio::detail::io_service_impl& io_service_;
    boost::shared_ptr<boost::asio::steady_timer> timer_;
    boost::asio::detail::op_queue<coproto_op> op_queue_;
    op_map op_map_;
    queue_type& in_queue_;
    queue_type& out_queue_;
    bool queue_started_;
};

template <typename D, template <typename> class Q> std::size_t coproto_service_impl<D, Q>::implementation_type::request_count_ = 0;

#endif /* COPROTO_SERVICE_IMPL_HPP_ */
