#pragma once

#include <memory>
#include <string>

class BaseClass :
    public std::enable_shared_from_this<BaseClass>
{
public:
    BaseClass();
    virtual ~BaseClass();
    virtual void doSomething(int index, std::string value) = 0;
};
