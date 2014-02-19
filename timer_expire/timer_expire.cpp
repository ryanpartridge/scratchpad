/*
 * function_pointer.cpp
 *
 *  Created on: Jan 24, 2014
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace std;

typedef boost::function<void(void)> voidVoidFunc;
typedef boost::function<void(int)> voidIntFunc;

void func1(int anInt)
{
    cout << "func1: " << anInt << endl;
}

int main(int argc, char* argv[])
{
    cout << "entering main" << endl;
    voidIntFunc bound1;
    cout << "trying bound1 before initializing" << endl;
    if (bound1)
    {
        bound1(1);
    }
    else
    {
        cout << "did not execute" << endl;
    }

    bound1 = voidIntFunc(func1);
    cout << "trying bound1 after initializing" << endl;
    if (bound1)
    {
        bound1(1);
    }
    else
    {
        cout << "did not execute" << endl;
    }

    bound1.clear();
    cout << "trying bound1 after clearing" << endl;
    if (bound1)
    {
        bound1(1);
    }
    else
    {
        cout << "did not execute" << endl;
    }

    bound1 = boost::bind(&func1, _1);
    cout << "trying bound1 after binding without value" << endl;
    if (bound1)
    {
        bound1(1);
    }
    else
    {
        cout << "did not execute" << endl;
    }

    bound1 = boost::bind(&func1, 7);
    cout << "trying bound1 after binding with value" << endl;
    if (bound1)
    {
        bound1(1);
    }
    else
    {
        cout << "did not execute" << endl;
    }

    voidVoidFunc bound2(boost::bind(&func1, 1));
    cout << "trying bound2 after binding with value" << endl;
    if (bound2)
    {
        bound2();
    }
    else
    {
        cout << "did not execute" << endl;
    }

    cout << "exiting main" << endl;
    return 0;
}
