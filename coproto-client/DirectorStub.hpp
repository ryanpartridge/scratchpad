/*
 * DirectorStub.hpp
 *
 *  Created on: Sep 20, 2016
 *      Author: rpartridge
 */

#ifndef DIRECTORSTUB_HPP_
#define DIRECTORSTUB_HPP_

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

#include <IDirector.hpp>

class DirectorStub : public IDirector
{
public:
    explicit DirectorStub(boost::asio::io_service& io_service);
    virtual ~DirectorStub();

    virtual std::string getValue(const std::string& name, boost::asio::yield_context yield);
    virtual bool isSet(const std::string& name, boost::asio::yield_context yield);

private:
    boost::asio::io_service& io_service_;
};

#endif /* DIRECTORSTUB_HPP_ */
