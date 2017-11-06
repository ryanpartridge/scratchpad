#include <iostream>
#include <Foo.hpp>

Foo::Foo()
{
}

void Foo::doFoo()
{
    std::cout << "do foo" << std::endl;
    bar = new Bar();
    bar->doBar();
    delete bar;
}
