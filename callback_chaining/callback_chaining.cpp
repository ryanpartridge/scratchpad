/*
 * callback_chaining.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>

typedef boost::function<void(int, const std::string)> LowLevelCallback;
typedef boost::function<void(const std::string, bool)> HighLevelCallback;

void kicker(LowLevelCallback llc)
{
    llc(1, "hello");
}

void lowLevelFunc(int intVal, const std::string strVal, HighLevelCallback hlc)
{
    std::cout << "[lowlevelFunc] intVal: " << intVal << " strVal: " << strVal << std::endl;
    hlc(strVal, true);
}

void highLevelFunc(const std::string strVal, bool boolVal)
{
    std::cout << "[highLevelFunc] strVal: " << strVal << " boolVal: " << boolVal << std::endl;
}

int main(int argc, char* argv[])
{
    boost::asio::io_service io_service;
    std::cout << "[main] calling run" << std::endl;
    kicker(boost::bind(lowLevelFunc, _1, _2, highLevelFunc));

    io_service.run();
    std::cout << "[main] run complete" << std::endl;
    return 0;
}
