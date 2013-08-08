/*
 * handler_posting.cpp
 *
 *  Created on: Aug 8, 2013
 *      Author: rpartridge
 */

#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/ref.hpp>
#include <boost/chrono.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind/protect.hpp>

#include <Task.hpp>

using namespace std;

void freeFunction()
{
    cout << "free function response handler" << endl;
}

void freeFunctionWithParam(int i)
{
    cout << "free function response handler with parameter (" << i << ")" << endl;
}

class Functor
{
public:
    typedef void result_type;

    void operator()()
    {
        cout << "functor response handler" << endl;
    }

    void operator()(int i)
    {
        cout << "functor response handler with parameter (" << i << ")" << endl;
    }
};

class MemberFunction
{
public:
    void handlerFunction()
    {
        cout << "member function response handler" << endl;
    }

    void handlerFunctionWithParam(int i)
    {
        cout << "member function response handler with parameter (" << i << ")" << endl;
    }
};

int main(int argc, char* argv[])
{
    boost::asio::io_service workerService, responderService;
    boost::asio::io_service::work workerWork(workerService), responderWork(responderService);

    // start the responder thread first so it's ready when the worker
    // thread sends it a response
    boost::thread responder(boost::bind(&boost::asio::io_service::run, boost::ref(responderService)));
    boost::thread worker(boost::bind(&boost::asio::io_service::run, boost::ref(workerService)));

    boost::shared_ptr<Task> t = boost::make_shared<Task>(boost::ref(workerService), boost::ref(responderService));

    // these work, but do not post to the io_service, which is the whole point
    //t->doWork(3, freeFunction); // works
    //t->doWork(3, Functor()); // works
    //t->doWork(3, boost::bind(&MemberFunction::handlerFunction, MemberFunction()));

    // these forms all work
    //workerService.post(boost::bind(&Task::doWork<void(&)()>, t, 3, boost::ref(freeFunction))); // works
    //workerService.post(boost::bind(&Task::doWork<Functor>, t, 3, Functor())); // works
    //workerService.post(boost::bind(&Task::doWork<boost::function<void()> >, t, 3, boost::protect(boost::bind(&MemberFunction::handlerFunction, MemberFunction())))); // works

    //workerService.post(boost::bind(&Task::doWorkWithParam<void(&)(int)>, t, 3, boost::ref(freeFunctionWithParam))); // works
    //workerService.post(boost::bind<void>(&Task::doWorkWithParam<Functor>, t, 3, Functor())); // works
    //workerService.post(boost::bind(&Task::doWorkWithParam<boost::function<void(int)> >, t, 3, boost::protect(boost::bind(&MemberFunction::handlerFunctionWithParam, MemberFunction(), _1)))); //works

    // shut down the worker thread
    cout << "waiting for worker thread to finish" << endl;
    if (!worker.try_join_for(boost::chrono::seconds(5)))
    {
        cout << "worker thread still going, stopping the service" << endl;
        workerService.stop();
        cout << "waiting for worker thread to finish (second time)" << endl;
        if (!worker.try_join_for(boost::chrono::seconds(1)))
        {
            cout << "worker thread still going, interrupting the thread" << endl;
            worker.interrupt();
            cout << "waiting for worker thread to finish (final time)" << endl;
            worker.try_join_for(boost::chrono::seconds(1));
        }
        else
        {
            cout << "worker thread finished" << endl;
        }
    }
    else
    {
        cout << "worker thread finished" << endl;
    }

    // shut down the responder thread
    cout << endl;
    cout << "waiting for responder thread to finish" << endl;
    if (!responder.try_join_for(boost::chrono::seconds(5)))
    {
        cout << "responder thread still going, stopping the service" << endl;
        responderService.stop();
        cout << "waiting for responder thread to finish (second time)" << endl;
        if (!responder.try_join_for(boost::chrono::seconds(1)))
        {
            cout << "responder thread still going, interrupting the thread" << endl;
            responder.interrupt();
            cout << "waiting for responder thread to finish (final time)" << endl;
            responder.try_join_for(boost::chrono::seconds(1));
        }
        else
        {
            cout << "responder thread finished" << endl;
        }
    }
    else
    {
        cout << "responder thread finished" << endl;
    }

    cout << endl;
    cout << "all work completed -- exiting" << endl;
    return 0;
}
