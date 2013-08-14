/*
 * thread_interrupt.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: rpartridge
 */
#include <iostream>
#include <math.h>
#include <boost/cstdint.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

void wasteTime()
{
    boost::uint64_t val = 0, max = (boost::uint64_t) (pow(2, 64) - 1);
    try
    {
        while (val < max)
        {
            ++val;
            if ((val % 1000000) == 0)
            {
                boost::this_thread::interruption_point();
            }
        }
    }
    catch (const boost::thread_interrupted&)
    {
        std::cout << "counting has been interrputed" << std::endl;
    }
    std::cout << "done counting with value: " << val << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "staring up" << std::endl;
    std::cout << std::endl;
    boost::thread worker(&wasteTime);
    if (!worker.try_join_for(boost::chrono::seconds(1)))
    {
        std::cout << "interrupting thread" << std::endl;
        worker.interrupt();
    }
    worker.join();
    std::cout << std::endl;

    std::cout << "creating a \"not\" thread" << std::endl;
    boost::thread notThread;
    std::cout << "joining the \"not\" thread" << std::endl;
    notThread.join();
    std::cout << std::endl;

    std::cout << "creating an empty thread shared_ptr" << std::endl;
    boost::shared_ptr<boost::thread> ptrThread; // = boost::make_shared<boost::thread>();
    std::cout << "joining the empty thread shared_ptr" << std::endl;
    if (ptrThread)
    {
        ptrThread->join();
    }
    ptrThread.reset();
    std::cout << std::endl;

    std::cout << "creating a \"not\" thread shared_ptr" << std::endl;
    ptrThread = boost::make_shared<boost::thread>();
    std::cout << "joining the \"not\" thread shared_ptr" << std::endl;
    if (ptrThread)
    {
        ptrThread->join();
    }

    std::cout << std::endl;
    std::cout << "creating a worker thread shared_ptr" << std::endl;
    ptrThread = boost::make_shared<boost::thread>(&wasteTime);
    std::cout << "joining the worker thread shared_ptr" << std::endl;
    if (ptrThread)
    {
        if (!ptrThread->try_join_for(boost::chrono::seconds(1)))
        {
            std::cout << "interrupting thread" << std::endl;
            ptrThread->interrupt();
        }
        ptrThread->join();
    }
    std::cout << std::endl;

    std::cout << "all done" << std::endl;
    return 0;
}

