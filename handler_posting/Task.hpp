/*
 * Task.hpp
 *
 *  Created on: Aug 8, 2013
 *      Author: rpartridge
 */

#ifndef TASK_HPP_
#define TASK_HPP_

#include <BasicTask.hpp>
#include <BasicTask2.hpp>
#include <BasicTask3.hpp>

class FunctorBase :
    public boost::enable_shared_from_this<FunctorBase>,
    private boost::noncopyable
{
public:
    FunctorBase()
    {

    }
    virtual ~FunctorBase()
    {

    }

    virtual void operator()() = 0;
    virtual void operator()(int) = 0;
};

//typedef BasicTask<> Task;
typedef BasicTask Task;
typedef BasicTask2<void (&)(int)> FreeFunctionTask;
typedef BasicTask2<FunctorBase&> FunctorTask;
typedef BasicTask2<boost::function<void(int)> > MemberFunctionTask;
typedef BasicTask2<boost::function<void(int)> > BoostFunctionTask;

#endif /* TASK_HPP_ */
