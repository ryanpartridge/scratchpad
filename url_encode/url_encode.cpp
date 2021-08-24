#include <iostream>
#include <cstdint>
#include <string>

std::string encode(std::string const& input)
{
    return std::string(input);
}

std::string decode(std::string const& input)
{
    std::string decoded(input);
    for (auto c: decoded)
    {
        std::cout << c << std::endl;
    }
    return decoded;
}

int main(int argc, char* argv[])
{
    std::cout << "url_encode program" << std::endl;

    std::string encoded("url%20encoded");
    std::cout << "encoded: " << encoded << std::endl;
    std::cout << "decoded: " << decode(encoded) << std::endl;

/*
    uint32_t c{' '};
    std::cout << "character: " << static_cast<char>(++c) << " (" << std::hex << c << ")" << std::endl;
    std::cout << "character: " << static_cast<char>(++c) << " (" << std::hex << c << ")" << std::endl;
    std::cout << "character: " << static_cast<char>(++c) << " (" << std::hex << c << ")" << std::endl;
*/

    return 0;
}
