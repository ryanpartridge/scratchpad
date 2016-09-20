/*
 * coproto_op.hpp
 *
 *  Created on: Sep 8, 2016
 *      Author: rpartridge
 */

#ifndef COPROTO_OP_HPP_
#define COPROTO_OP_HPP_

#include <boost/asio/detail/operation.hpp>

class coproto_op : public boost::asio::detail::operation
{
public:
    // error code to be passed to the completion handler
    boost::system::error_code ec_;

    // value passed to the completion handler
    std::string value_;

protected:
    coproto_op(func_type func) :
        boost::asio::detail::operation(func)
    {
    }
};

#endif /* COPROTO_OP_HPP_ */
