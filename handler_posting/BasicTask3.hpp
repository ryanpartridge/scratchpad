/*
 * BasicTask3.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: ryan
 */

#ifndef BASICTASK3_HPP_
#define BASICTASK3_HPP_

#include <iostream>

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/bind.hpp>
#include <boost/bind/protect.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>

//typedef void (*VoidIntFunction)(int);
typedef boost::function<void(int)> CompletionHandler;
typedef boost::shared_ptr<CompletionHandler> SP_CompletionHandler;

class BasicTask3 :
    public boost::enable_shared_from_this<BasicTask3>,
    private boost::noncopyable
{
public:
    BasicTask3(boost::asio::io_service& workerService, boost::asio::io_service& responseService, CompletionHandler handler) :
        workerService_(workerService),
        responseService_(responseService),
        handler_(handler)
    {
        std::cout << "BasicTask3 constructor" << std::endl;
    }

    virtual ~BasicTask3()
    {
        std::cout << "BasicTask3 destructor" << std::endl;
    }

    inline void doWorkWithParam(int seconds)
    {
        std::cout << "[BasicTask3] doing work for " << seconds << " seconds" << std::endl;
        boost::this_thread::sleep_for(boost::chrono::seconds(seconds));
        std::cout << "[BasicTask3] completed work" << std::endl;
        responseService_.post(boost::bind(handler_, seconds));
    }

    // wrap a free function in a CompletionHandler
//    static SP_CompletionHandler wrapHandler(VoidIntFunction& rawHandler)
//    {
//        return boost::make_shared<CompletionHandler>(boost::ref(rawHandler));
//    }

    // wrap a function pointer or functor in a CompletionHandler
    template <typename FunctionObject>
    static CompletionHandler wrapHandler(FunctionObject& funcObj)
    {
        return CompletionHandler(boost::ref(funcObj));
    }

    // wrap an Object instance and member function in a CompletionHandler
    template <typename HandlerObject, typename HandlerFunction>
    static CompletionHandler wrapHandler(HandlerObject& handlerObject, HandlerFunction handlerFunction)
    {
        return CompletionHandler(boost::bind(handlerFunction, boost::ref(handlerObject), _1));
    }

    // create a shared pointer to an object and wrap it and the specified member function in a CompletionHandler
    template <typename HandlerType, typename HandlerFunction>
    static CompletionHandler wrapHandler(HandlerFunction handlerFunction)
    {
        boost::shared_ptr<HandlerType> handlerObject = boost::make_shared<HandlerType>();
        return CompletionHandler(boost::bind(handlerFunction, handlerObject, _1));
    }

private:
    boost::asio::io_service& workerService_;
    boost::asio::io_service& responseService_;
    CompletionHandler handler_;
};

#endif /* BASICTASK3_HPP_ */
