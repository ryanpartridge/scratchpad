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

class TaskFactory :
    private boost::noncopyable
{
public:
    TaskFactory(boost::asio::io_service& workerService, boost::asio::io_service& responseService) :
        workerService_(workerService),
        responseService_(responseService)
    {
        std::cout << "[TaskFactory] constructor" << std::endl;
    }

    virtual ~TaskFactory()
    {
        std::cout << "[TaskFactory] destructor" << std::endl;
    }

    static TaskFactory& Instance()
    {
        return *instance_;
    }

    static TaskFactory& Construct(boost::asio::io_service& workerService, boost::asio::io_service& responseService)
    {
        if (!instance_)
        {
            instance_ = boost::make_shared<TaskFactory>(boost::ref(workerService), boost::ref(responseService));
        }
        return *instance_;
    }

    static void Destroy()
    {
        instance_.reset();
    }

private:
    static boost::shared_ptr<TaskFactory> instance_;
    boost::asio::io_service& workerService_;
    boost::asio::io_service& responseService_;
};

boost::shared_ptr<TaskFactory> TaskFactory::instance_;

#endif /* TASKFACTORY_HPP_ */
