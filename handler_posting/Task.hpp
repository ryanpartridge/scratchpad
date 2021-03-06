/*
 * Task.hpp
 *
 *  Created on: Aug 8, 2013
 *      Author: rpartridge
 */

#ifndef TASK_HPP_
#define TASK_HPP_

#include <iostream>

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>

//typedef void (*VoidIntFunction)(int);
typedef boost::function<void(int)> CompletionHandler;

class Task :
    public boost::enable_shared_from_this<Task>,
    private boost::noncopyable
{
public:
    Task(boost::asio::io_service& workerService, boost::asio::io_service& responseService, CompletionHandler handler) :
        workerService_(workerService),
        responseService_(responseService),
        handler_(handler)
    {
        std::cout << "[Task] constructor" << std::endl;
    }

    virtual ~Task()
    {
        std::cout << "[Task] destructor" << std::endl;
    }

    inline void doWorkWithParam(int seconds)
    {
        std::cout << "[Task] doing work for " << seconds << " seconds" << std::endl;
        boost::this_thread::sleep_for(boost::chrono::seconds(seconds));
        std::cout << "[Task] completed work" << std::endl;
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

    // wrap a raw pointer to an object and a member function in a CompletionHandler
    template <typename HandlerType, typename HandlerFunction>
    static CompletionHandler wrapHandler(HandlerType* handlerPointer, HandlerFunction handlerFunction)
    {
        return CompletionHandler(boost::bind(handlerFunction, handlerPointer, _1));
    }

    // wrap a shared pointer to an object and a member function in a CompletionHandler
    template <typename HandlerType, typename HandlerFunction>
    static CompletionHandler wrapHandler(boost::shared_ptr<HandlerType> handlerPointer, HandlerFunction handlerFunction)
    {
        return CompletionHandler(boost::bind(handlerFunction, handlerPointer, _1));
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

#endif /* TASK_HPP_ */
