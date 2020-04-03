#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "char length:\t\t" << (sizeof(char) * 8) << std::endl;
    std::cout << "short length:\t\t" << (sizeof(short) * 8) << std::endl;
    std::cout << "int length:\t\t" << (sizeof(int) * 8) << std::endl;
    std::cout << "long length:\t\t" << (sizeof(long) * 8) << std::endl;
    std::cout << "long long length:\t" << (sizeof(long long) * 8) << std::endl;

    return 0;
}