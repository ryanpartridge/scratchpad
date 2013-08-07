/*
 * template_test.cpp
 *
 *  Created on: Aug 7, 2013
 *      Author: rpartridge
 */
#include <iostream>

#include <TemplateFuncClass.hpp>

using namespace std;

class FuncClass
{
public:
    void operator()()
    {
        cout << "operator()()" << endl;
    }

    void operator()(int i)
    {
        cout << "operator()(int)" << endl;
    }

    void operator()(char* s)
    {
        cout << "operator()(char* s)" << endl;
    }

    void operator()(string s)
    {
        cout << "operator()(string)" << endl;
    }
};

void foo()
{

}

int main(int argc, char* argv[])
{
    TemplateFuncClass c;
    c.execFunc(FuncClass());
    return 0;
}
