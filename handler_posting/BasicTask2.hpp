/*
 * BasicTask2.hpp
 *
 *  Created on: Aug 8, 2013
 *      Author: rpartridge
 */

#ifndef BASICTASK2_HPP_
#define BASICTASK2_HPP_

#include <iostream>

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

template <typename CompletionHandler>
class BasicTask2 :
    public boost::enable_shared_from_this<BasicTask2<CompletionHandler> >,
    private boost::noncopyable
{
public:
    explicit BasicTask2(boost::asio::io_service& workerService, boost::asio::io_service& responseService, CompletionHandler& handler) :
        workerService_(workerService),
        responseService_(responseService),
        handler_(handler)
    {

    }

    inline void doWork(int seconds)
    {
        std::cout << "[BasicTask2] doing work for " << seconds << " seconds" << std::endl;
        boost::this_thread::sleep_for(boost::chrono::seconds(seconds));
        std::cout << "[BasicTask2] completed work" << std::endl;
        responseService_.post(handler_);
    }

    inline void doWorkWithParam(int seconds)
    {
        std::cout << "[BasicTask2] doing work for " << seconds << " seconds" << std::endl;
        boost::this_thread::sleep_for(boost::chrono::seconds(seconds));
        std::cout << "[BasicTask2] completed work" << std::endl;
        responseService_.post(boost::bind(handler_, seconds));
    }

private:
    boost::asio::io_service& workerService_;
    boost::asio::io_service& responseService_;
    CompletionHandler& handler_;
};
#endif /* BASICTASK_HPP_ */
