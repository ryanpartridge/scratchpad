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
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

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

class Functor :
    public FunctorBase
{
public:
    typedef void result_type;

    Functor() :
        FunctorBase()
    {
        cout << "Functor default constructor" << endl;
    }

    virtual ~Functor()
    {
        cout << "Functor destructor" << endl;
    }

    void operator()()
    {
        cout << "functor response handler" << endl;
    }

    void operator()(int i)
    {
        cout << "functor response handler with parameter (" << i << ")" << endl;
    }
};

class MemberFunction :
    public boost::enable_shared_from_this<MemberFunction>,
    private boost::noncopyable
{
public:
    MemberFunction()
    {
        cout << "MemberFunction default constructor" << endl;
    }

    virtual ~MemberFunction()
    {
        cout << "MemberFunction destructor" << endl;
    }

    void handlerFunction()
    {
        cout << "member function response handler" << endl;
    }

    void handlerFunctionWithParam(int i)
    {
        cout << "member function response handler with parameter (" << i << ")" << endl;
    }
};

void testFreeFunctionCompletionLifeCycle(boost::asio::io_service& workerService, boost::asio::io_service& responderService)
{
    CompletionHandler handler = BasicTask3::wrapHandler(freeFunctionWithParam);
    boost::shared_ptr<BasicTask3> bt = boost::make_shared<BasicTask3>(boost::ref(workerService), boost::ref(responderService), handler);
    workerService.post(boost::bind(&BasicTask3::doWorkWithParam, bt, 3));
}

void testMemberFunctionCompletionLifeCycle(boost::asio::io_service& workerService, boost::asio::io_service& responderService)
{
    boost::shared_ptr<MemberFunction> mf = boost::make_shared<MemberFunction>();
    CompletionHandler handler(boost::bind(&MemberFunction::handlerFunctionWithParam, mf, _1));
    //CompletionHandler handler = BasicTask3(mf, &MemberFunction::handlerFunctionWithParam);
    boost::shared_ptr<BasicTask3> bt = boost::make_shared<BasicTask3>(boost::ref(workerService), boost::ref(responderService), handler);
    //BasicTask3 bt(workerService, responderService, handler);
    workerService.post(boost::bind(&BasicTask3::doWorkWithParam, bt, 3));

//    Functor f;
//    CompletionHandler handler2(boost::ref(f));
}

void testSharedPointerMemberFunctionCompletionLifeCycle(boost::asio::io_service& workerService, boost::asio::io_service& responderService)
{
    CompletionHandler handler = BasicTask3::wrapHandler<MemberFunction>(&MemberFunction::handlerFunctionWithParam);
    boost::shared_ptr<BasicTask3> bt = boost::make_shared<BasicTask3>(boost::ref(workerService), boost::ref(responderService), handler);
    workerService.post(boost::bind(&BasicTask3::doWorkWithParam, bt, 3));
}

int main(int argc, char* argv[])
{
    boost::asio::io_service workerService, responderService;
    boost::asio::io_service::work workerWork(workerService), responderWork(responderService);

    // start the responder thread first so it's ready when the worker
    // thread sends it a response
    boost::thread responder(boost::bind(&boost::asio::io_service::run, boost::ref(responderService)));
    boost::thread worker(boost::bind(&boost::asio::io_service::run, boost::ref(workerService)));

    //boost::shared_ptr<Task> t = boost::make_shared<Task>(boost::ref(workerService), boost::ref(responderService));

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

    //boost::shared_ptr<FreeFunctionTask> fft = boost::make_shared<FreeFunctionTask>(boost::ref(workerService), boost::ref(responderService), boost::ref(freeFunctionWithParam));
    //workerService.post(boost::bind(&FreeFunctionTask::doWorkWithParam, fft, 3)); // works

    //Functor f;
    //boost::shared_ptr<FunctorTask> ft = boost::make_shared<FunctorTask>(boost::ref(workerService), boost::ref(responderService), boost::ref(f));
    //workerService.post(boost::bind(&FunctorTask::doWorkWithParam, ft, 3));

    //MemberFunction mfi;
    //boost::function<void(int)> mf = boost::bind(&MemberFunction::handlerFunctionWithParam, boost::ref(mfi), _1);
    //boost::shared_ptr<MemberFunctionTask> mft = boost::make_shared<MemberFunctionTask>(boost::ref(workerService), boost::ref(responderService), boost::ref(mf));
    //workerService.post(boost::bind(&MemberFunctionTask::doWorkWithParam, mft, 3)); // works

//    CompletionHandler freeFunc(freeFunctionWithParam);
//    Functor f;
//    CompletionHandler functorFunc(boost::ref(f));
//    MemberFunction mf;
//    CompletionHandler memberFunc(boost::bind(&MemberFunction::handlerFunctionWithParam, boost::ref(mf), _1));

    //boost::shared_ptr<BoostFunctionTask> bft = boost::make_shared<BoostFunctionTask>(boost::ref(workerService), boost::ref(responderService), boost::ref(freeFunc)); // works
    //boost::shared_ptr<BoostFunctionTask> bft = boost::make_shared<BoostFunctionTask>(boost::ref(workerService), boost::ref(responderService), boost::ref(functorFunc)); // works
    //boost::shared_ptr<BoostFunctionTask> bft = boost::make_shared<BoostFunctionTask>(boost::ref(workerService), boost::ref(responderService), boost::ref(memberFunc)); // works
    //workerService.post(boost::bind(&BoostFunctionTask::doWorkWithParam, bft, 3));

    // free function
    //CompletionHandler handler = BasicTask3::wrapHandler(freeFunctionWithParam); // works

    // Functor
//    Functor f;
//    SP_CompletionHandler handler = BasicTask3::wrapHandler(f); // works

    // member function
//    MemberFunction mf;
//    SP_CompletionHandler handler = BasicTask3::wrapHandler(mf, &MemberFunction::handlerFunctionWithParam); // works

    // member function that uses a shared_ptr
//    SP_CompletionHandler handler = BasicTask3::wrapHandler<MemberFunction>(&MemberFunction::handlerFunctionWithParam);

    // schedule the work
//    boost::shared_ptr<BasicTask3> bt = boost::make_shared<BasicTask3>(boost::ref(workerService), boost::ref(responderService), handler);
//    workerService.post(boost::bind(&BasicTask3::doWorkWithParam, bt, 3));

//    cout << "calling free function life cycle test" << endl;
//    testFreeFunctionCompletionLifeCycle(workerService, responderService);
//    cout << "returned from free function life cycle test" << endl;
//    cout << endl;
//
//    cout << "calling member function life cycle test" << endl;
//    testMemberFunctionCompletionLifeCycle(workerService, responderService);
//    cout << "returned from member function life cycle test" << endl;
//    cout << endl;

    cout << "calling shared pointer member function life cycle test" << endl;
    testSharedPointerMemberFunctionCompletionLifeCycle(workerService, responderService);
    cout << "returned from shared pointer member function life cycle test" << endl;
    cout << endl;

    boost::this_thread::sleep_for(boost::chrono::seconds(5));

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
