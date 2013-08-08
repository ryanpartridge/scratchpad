/*
 * BasicTask.hpp
 *
 *  Created on: Aug 8, 2013
 *      Author: rpartridge
 */

#ifndef BASICTASK_HPP_
#define BASICTASK_HPP_

#include <iostream>

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

class BasicTask :
    public boost::enable_shared_from_this<BasicTask>,
    private boost::noncopyable
{
public:
    explicit BasicTask(boost::asio::io_service& workerService, boost::asio::io_service& responseService) :
        workerService_(workerService),
        responseService_(responseService)
    {

    }

    template <typename CompletionHandler>
    inline void doWork(int seconds, CompletionHandler handler)
    {
        std::cout << "[BasicTask] doing work for " << seconds << " seconds" << std::endl;
        boost::this_thread::sleep_for(boost::chrono::seconds(seconds));
        std::cout << "[BasicTask] completed work" << std::endl;
        responseService_.post(handler);
    }

    template <typename CompletionHandler>
    inline void doWorkWithParam(int seconds, CompletionHandler handler)
    {
        std::cout << "[BasicTask] doing work for " << seconds << " seconds" << std::endl;
        boost::this_thread::sleep_for(boost::chrono::seconds(seconds));
        std::cout << "[BasicTask] completed work" << std::endl;
        responseService_.post(boost::bind(handler, seconds));
    }

private:
    boost::asio::io_service& workerService_;
    boost::asio::io_service& responseService_;
};
#endif /* BASICTASK_HPP_ */
