#pragma once

#include <BaseClass.hpp>

class Concrete :
    public BaseClass
{
public:
    Concrete();
    virtual ~Concrete();
    virtual void doSomething(int index, std::string value) override;
};
