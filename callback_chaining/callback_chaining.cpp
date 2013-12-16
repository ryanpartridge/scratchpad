/*
 * callback_chaining.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/function.hpp>

typedef boost::function<void(int, const std::string)> LowLevelCallback;
typedef boost::function<void(const std::string, bool)> HighLevelCallback;
typedef boost::function<void(void)> voidFunc;

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

void noLevelFunc()
{
    std::cout << "[noLevelFunc]" << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "[main] running" << std::endl;
    kicker(boost::bind(lowLevelFunc, _1, _2, highLevelFunc));

    voidFunc f = noLevelFunc;
    f();

    std::cout << "[main] complete" << std::endl;
    return 0;
}
