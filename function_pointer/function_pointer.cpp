/*
 * function_pointer.cpp
 *
 *  Created on: Jan 24, 2014
 *      Author: rpartridge
 */

#include <iostream>
#include <functional>
#include <boost/function.hpp>
#include <boost/bind/bind.hpp>

using voidVoidFunc = boost::function<void(void)>;
using voidIntFunc = boost::function<void(int)>;

void func1(int anInt)
{
    std::cout << "func1: " << anInt << std::endl;
}

void func2(int anInt, bool aBool)
{
    std::cout << "func2: " << anInt << ", " << std::boolalpha << aBool << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "entering main" << std::endl;
    voidIntFunc bound1;
    std::cout << "trying bound1 before initializing" << std::endl;
    if (bound1)
    {
        bound1(1);
    }
    else
    {
        std::cout << "did not execute" << std::endl;
    }

    bound1 = voidIntFunc(func1);
    std::cout << "trying bound1 after initializing" << std::endl;
    if (bound1)
    {
        bound1(1);
    }
    else
    {
        std::cout << "did not execute" << std::endl;
    }

    bound1.clear();
    std::cout << "trying bound1 after clearing" << std::endl;
    if (bound1)
    {
        bound1(1);
    }
    else
    {
        std::cout << "did not execute" << std::endl;
    }

    bound1 = boost::bind(&func1, boost::placeholders::_1);
    std::cout << "trying bound1 after binding without value" << std::endl;
    if (bound1)
    {
        bound1(1);
    }
    else
    {
        std::cout << "did not execute" << std::endl;
    }

    bound1 = boost::bind(&func1, 7);
    std::cout << "trying bound1 after binding with value" << std::endl;
    if (bound1)
    {
        bound1(1);
    }
    else
    {
        std::cout << "did not execute" << std::endl;
    }

    voidVoidFunc bound2(boost::bind(&func1, 1));
    std::cout << "trying bound2 after binding with value" << std::endl;
    if (bound2)
    {
        bound2();
    }
    else
    {
        std::cout << "did not execute" << std::endl;
    }

    std::function<void(void)> standardFunc = bound2;
    std::cout << "trying standardFunc after converting from boost::function" << std::endl;
    if (standardFunc)
    {
        standardFunc();
    }
    else
    {
        std::cout << "did not execute" << std::endl;
    }

    voidVoidFunc backToBoost = standardFunc;
    std::cout << "trying backToBoost after converting from std::function" << std::endl;
    if (backToBoost)
    {
        backToBoost();
    }
    else
    {
        std::cout << "did not execute" << std::endl;
    }

    std::cout << "exiting main" << std::endl;

    std::function<void(int)> boundParameter = std::bind(&func2, std::placeholders::_1, true);
    std::cout << "trying boundParameter passing first parameter and bound second parameter" << std::endl;
    if (boundParameter)
    {
        boundParameter(42);
    }
    else
    {
        std::cout << "did not execute" << std::endl;
    }

    return 0;
}
