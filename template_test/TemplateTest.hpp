/*
 * TemplateTest.hpp
 *
 *  Created on: Aug 7, 2013
 *      Author: rpartridge
 */

#pragma once

#include <string>

class TemplateTest
{
public:
    TemplateTest();
    virtual ~TemplateTest();

    template<class Func>
    void execFunc(Func func)
    {
        auto value = std::string("hello");
        func(value);
    }
};
