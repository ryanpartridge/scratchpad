#ifndef FOO_HPP_
#define FOO_HPP_

#include <Bar.hpp>

class Foo
{
public:
    Foo();
    void doFoo();

private:
    Bar* bar;
};

#endif
