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

void wasteTimeNoInterrupt()
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
    catch (boost::thread_interrupted&)
    {
        std::cout << "counting has been interrputed" << std::endl;
    }
    std::cout << "done counting with value: " << val << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "staring up" << std::endl;
    boost::thread worker(&wasteTimeNoInterrupt);
    if (!worker.try_join_for(boost::chrono::seconds(5)))
    {
        std::cout << "interrupting thread" << std::endl;
        worker.interrupt();
    }
    worker.join();

    return 0;
}

