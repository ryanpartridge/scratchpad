#include <iostream>
#include <future>

int get_value()
{
    return 10;
}

int main(int32_t argc, char* argv[])
{
    std::future<int> foo = std::async(get_value);
    if (foo.valid())
    {
        std::cout << "value: " << foo.get() << std::endl;
    }
    else
    {
        std::cout << "foo is not valid" << std::endl;
    }

    if (foo.valid())
    {
        std::cout << "value (again): " << foo.get() << std::endl;
    }
    else
    {
        std::cout << "foo is no longer valid" << std::endl;
    }

    foo = std::async(get_value);
    if (foo.valid())
    {
        std::cout << "value: " << foo.get() << std::endl;
    }
    else
    {
        std::cout << "foo is not valid" << std::endl;
    }
    return 0;
}
