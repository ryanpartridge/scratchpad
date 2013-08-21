/*
 * TaskFactory.hpp
 *
 *  Created on: Aug 15, 2013
 *      Author: rpartridge
 */

#ifndef TASKFACTORY_HPP_
#define TASKFACTORY_HPP_

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>

#include <Task.hpp>

typedef boost::shared_ptr<Task> SP_Task;

class TaskFactory :
    private boost::noncopyable
{
private:
    TaskFactory(boost::asio::io_service& workerService, boost::asio::io_service& responseService) :
        workerService_(workerService),
        responseService_(responseService)
    {
        std::cout << "[TaskFactory] constructor" << std::endl;
    }

public:
    virtual ~TaskFactory()
    {
        std::cout << "[TaskFactory] destructor" << std::endl;
    }

    static void Construct(boost::asio::io_service& workerService, boost::asio::io_service& responseService)
    {
        if (!instance_)
        {
            // cannot use make_shared here because the constructor is private
            instance_ = boost::shared_ptr<TaskFactory>(new TaskFactory(workerService, responseService));
        }
    }

    static void Destroy()
    {
        instance_.reset();
    }

    // wrap a function pointer or functor
    template <typename FunctionObject>
    static SP_Task createTask(FunctionObject& funcObj)
    {
        SP_Task task;
        if (instance_)
        {
            CompletionHandler handler = Task::wrapHandler(funcObj);
            task = boost::make_shared<Task>(boost::ref(instance_->workerService_), boost::ref(instance_->responseService_), handler);
        }
        return task;
    }

    // wrap an Object instance and member function
    template <typename HandlerObject, typename HandlerFunction>
    static SP_Task createTask(HandlerObject& handlerObject, HandlerFunction handler)
    {
        SP_Task task;
        if (instance_)
        {
            CompletionHandler handler = Task::wrapHandler(handlerObject, handler);
            task = boost::make_shared<Task>(boost::ref(instance_->workerService_), boost::ref(instance_->responseService_), handler);
        }
        return task;
    }

    // wrap a raw pointer to an Object instance and member function
    template <typename HandlerObject, typename HandlerFunction>
    static SP_Task createTask(HandlerObject* handlerPointer, HandlerFunction handler)
    {
        SP_Task task;
        if (instance_)
        {
            CompletionHandler handler = Task::wrapHandler(handlerPointer, handler);
            task = boost::make_shared<Task>(boost::ref(instance_->workerService_), boost::ref(instance_->responseService_), handler);
        }
        return task;
    }

    // wrap a shared pointer to an Object instance and member function
    template <typename HandlerObject, typename HandlerFunction>
    static SP_Task createTask(boost::shared_ptr<HandlerObject> handlerPointer, HandlerFunction handler)
    {
        SP_Task task;
        if (instance_)
        {
            CompletionHandler handler = Task::wrapHandler(handlerPointer, handler);
            task = boost::make_shared<Task>(boost::ref(instance_->workerService_), boost::ref(instance_->responseService_), handler);
        }
        return task;
    }

private:
    static boost::shared_ptr<TaskFactory> instance_;
    boost::asio::io_service& workerService_;
    boost::asio::io_service& responseService_;
};

boost::shared_ptr<TaskFactory> TaskFactory::instance_;

#endif /* TASKFACTORY_HPP_ */
