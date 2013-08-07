/*
 * template_test.cpp
 *
 *  Created on: Aug 7, 2013
 *      Author: rpartridge
 */

#include <TemplateFuncClass.hpp>

void foo()
{

}

int main(int argc, char* argv[])
{
    TemplateFuncClass c;
    c.execFunc(foo);
    return 0;
}
