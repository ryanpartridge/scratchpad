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
#include <boost/ref.hpp>

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

void wrapAndExecute(voidIntFunc& intFunc, int value)
{
    voidIntFunc executeFunc = boost::bind(intFunc, boost::placeholders::_1);
    executeFunc(value);
}

using StdVoidIntFunc = std::function<void(int)>;

class FunctionTester
{
public:
    FunctionTester() = default;
    virtual ~FunctionTester() {}

    void integerFunction(int value)
    {
        std::cout << "integerFunction value: " << value << std::endl;
    }

    void doIntegerWork(int value)
    {
        StdVoidIntFunc func = std::bind(&FunctionTester::integerFunction, this, std::placeholders::_1);
        doTypeWork(func, value);
        doTemplateWork(&FunctionTester::integerFunction, value);
    }

private:
    void doTypeWork(StdVoidIntFunc intFunc, int value)
    {
        std::cout << "typed function parameter" << std::endl;
        auto worker = std::bind(intFunc, std::placeholders::_1);
        worker(value);
    }

    template <class F>
    void doTemplateWork(F intFunc, int value)
    {
        std::cout << "templated function parameter" << std::endl;
        auto worker = std::bind(intFunc, this, std::placeholders::_1);
        worker(value);
    }
};


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

    FunctionTester ft;
    ft.doIntegerWork(88);

    return 0;
}
