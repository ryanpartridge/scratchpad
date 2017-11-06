#include <iostream>
#include <Baz.hpp>

Baz::Baz()
{
}

void Baz::doBaz()
{
    std::cout << "do baz" << std::endl;
    bar = new Bar();
    bar->doBar();
    delete bar;
}
