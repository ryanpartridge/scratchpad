#include <iostream>

#include <Foo.hpp>
#include <Baz.hpp>

int main(int argc, char* argv[])
{
    std::cout << "Hello world!" << std::endl;
    Foo foo;
    foo.doFoo();
    Baz baz;
    baz.doBaz();
    return 0;
}
