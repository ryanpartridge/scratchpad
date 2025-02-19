#include <iostream>

int main(int argc, char* argv[])
{
#if defined(C4_FOO)
    std::cout << "C4_FOO is defined" << std::endl;
#else
    std::cout << "C4_FOO is not defined" << std::endl;
#endif
    return 0;
}
