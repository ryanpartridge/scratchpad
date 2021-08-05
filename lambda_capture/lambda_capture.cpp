#include <iostream>

class C
{
public:
    C()
    {
        std::cout << "default constructor" << std::endl;
    }

    C(C const& other)
    {
        std::cout << "copy constructor" << std::endl;
        i = other.i + 1;
    }

    ~C()
    {
        std::cout << "destructor for value: " << i << std::endl;
    }

    uint32_t i{0};
};

void capture(C const& c)
{
    std::cout << "going to define the lambda_function" << std::endl;
    auto lambda_function = [&c]()
    {
        std::cout << "capture c.i value: " << c.i << std::endl;
    };
    std::cout << "going to invoke the lambda_function" << std::endl;
    lambda_function();

    std::cout << "post-lambda_function c.i value: " << c.i << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "program started" << std::endl;
    C c;
    std::cout << "original c.i value: " << c.i << std::endl;
    capture(c);
    std::cout << "post-capture function c.i value: " << c.i << std::endl;
    std::cout << "program exiting" << std::endl;
    return 0;
}
