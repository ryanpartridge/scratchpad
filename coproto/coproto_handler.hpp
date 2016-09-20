/*
 * coproto_handler.hpp
 *
 *  Created on: Sep 8, 2016
 *      Author: rpartridge
 */

#ifndef COPROTO_HANDLER_HPP_
#define COPROTO_HANDLER_HPP_

#include <boost/asio/detail/addressof.hpp>
#include <boost/asio/detail/fenced_block.hpp>
#include <boost/asio/detail/handler_alloc_helpers.hpp>
#include <boost/asio/detail/handler_invoke_helpers.hpp>

#include <coproto_op.hpp>

template <typename Handler>
class coproto_handler : public coproto_op
{
public:
    BOOST_ASIO_DEFINE_HANDLER_PTR(coproto_handler);

    coproto_handler(Handler& handler)
        : coproto_op(&coproto_handler::do_complete),
          handler_(BOOST_ASIO_MOVE_CAST(Handler)(handler))
    {
    }

    static void do_complete(boost::asio::detail::io_service_impl* owner,
        boost::asio::detail::operation* base,
        const boost::system::error_code& /* ec */,
        std::size_t /* bytes_transferred */)
    {
        // take ownership
        coproto_handler* h(static_cast<coproto_handler*>(base));
        ptr p = { boost::asio::detail::addressof(h->handler_), h, h };

        BOOST_ASIO_HANDLER_COMPLETION((h));

        // copy the handler
        boost::asio::detail::binder2<Handler, boost::system::error_code, std::string>
            handler(h->handler_, h->ec_, h->value_);
        p.h = boost::asio::detail::addressof(handler.handler_);
        p.reset();

        // make the upcall
        if (owner)
        {
            boost::asio::detail::fenced_block(boost::asio::detail::fenced_block::half);
            BOOST_ASIO_HANDLER_INVOCATION_BEGIN((handler.arg1_, handler.arg2_));
            boost_asio_handler_invoke_helpers::invoke(handler, handler.handler_);
            BOOST_ASIO_HANDLER_INVOCATION_END;
        }
    }

private:
    Handler handler_;
};

#endif /* COPROTO_HANDLER_HPP_ */
