/*
 * DirectorImpl.hpp
 *
 *  Created on: Sep 16, 2016
 *      Author: rpartridge
 */

#ifndef DIRECTORIMPL_HPP_
#define DIRECTORIMPL_HPP_

#include <boost/asio.hpp>

#include <IDirector.hpp>

class DirectorImpl : public IDirector
{
public:
    DirectorImpl();
    virtual ~DirectorImpl();

    virtual std::string getValue(const std::string& name);
    void start();
    void startAcceptor(boost::system::error_code& ec);
    void acceptConnection(boost::asio::yield_context yield);

private:
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::endpoint endpoint_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif /* DIRECTORIMPL_HPP_ */
