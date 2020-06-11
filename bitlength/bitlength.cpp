#include <iostream>
#include <cstdint>

#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>

void myFunc(uint32_t foo)
{
    std::cout << "foo value: " << foo << std::endl;
}

void myFunc(uint64_t foo)
{
    std::cout << "foo value: " << foo << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "char length:\t\t\t" << (sizeof(char) * 8) << std::endl;
    std::cout << "short length:\t\t\t" << (sizeof(short) * 8) << std::endl;
    std::cout << "int length:\t\t\t" << (sizeof(int) * 8) << std::endl;
    std::cout << "unsigned length:\t\t" << (sizeof(unsigned) * 8) << std::endl;
    std::cout << "unsigned int length:\t\t" << (sizeof(unsigned int) * 8) << std::endl;
    std::cout << "size_t length:\t\t\t" << (sizeof(size_t) * 8) << std::endl;
    std::cout << "long length:\t\t\t" << (sizeof(long) * 8) << std::endl;
    std::cout << "unsigned long length:\t\t" << (sizeof(unsigned long) * 8) << std::endl;
    std::cout << "unsigned long int length:\t" << (sizeof(unsigned long int) * 8) << std::endl;
    std::cout << "long long length:\t\t" << (sizeof(long long) * 8) << std::endl;
    std::cout << "long long int length:\t\t" << (sizeof(long long int) * 8) << std::endl;
    std::cout << "unsigned long long length:\t" << (sizeof(unsigned long long) * 8) << std::endl;
    std::cout << "unsigned long long int length:\t" << (sizeof(unsigned long long int) * 8) << std::endl;

    std::string input("4294967296");
    auto val = boost::convert<int64_t>(input, boost::cnv::strtol()).value_or(0);
    std::cout << "Input value: " << input << "\t\tOutput value: " << val << std::endl;

    myFunc(static_cast<uint64_t>(42));

    std::string negValue("-2147483648");
    auto converted = boost::convert<int64_t>(negValue, boost::cnv::strtol()).value_or(0);
    std::cout << "Negative value: " << negValue << "\tOutput value: " << std::hex << static_cast<uint64_t>(converted) << "\tTruncated: " << std::hex << static_cast<uint32_t>(converted) << std::endl;

    return 0;
}
