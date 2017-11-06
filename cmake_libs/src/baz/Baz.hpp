#ifndef BAZ_HPP_
#define BAZ_HPP_

#include <Bar.hpp>

class Baz
{
public:
    Baz();
    void doBaz();

private:
    Bar* bar;
};

#endif
