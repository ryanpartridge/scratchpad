/*
 * thread_interrupt.cpp
 *
 *  Created on: Jul 25, 2013
 *      Author: rpartridge
 */
#include <iostream>
#include <boost/cstdint.hpp>

void wasteTimeNoInterrupt()
{
    boost::uint64_t val = 0;
}

int main(int argc, char* argv[])
{
    wasteTimeNoInterrupt();
    return 0;
}



