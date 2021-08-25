#include <iostream>
#include <cstdint>
#include <string>

std::string encode(std::string const& input)
{
    return std::string(input);
}

std::string decode(std::string const& input)
{
    auto index = size_t(0);
    auto lastIndex = index;
    auto length = input.size();
    std::string decoded;

    while (index < length)
    {
        index = input.find('%', lastIndex);
        if (index != std::string::npos)
        {
            decoded.append(input, lastIndex, index - lastIndex);
            ++index;
            if ((index + 2) < length)
            {
                auto value = input.substr(index, 2);
                try
                {
                    size_t badIndex = 0;
                    int code = std::stoi(value, &badIndex, 16);
                    if (badIndex == value.size())
                    {
                        decoded.append(1, code);
                    }
                    else
                    {
                        return decoded;
                    }
                }
                catch(std::exception const& ex)
                {
                    return decoded;
                }
                lastIndex = index + 2;
            }
            else
            {
                return decoded;
            }
        }
        else
        {
            decoded.append(input, lastIndex);
        }
    }

    return decoded;
}

int main(int argc, char* argv[])
{
    std::cout << "url_encode program" << std::endl;

    std::string encoded{"url%20encoded%22string"};
    std::cout << "encoded: " << encoded << std::endl;
    std::cout << "decoded: " << decode(encoded) << std::endl;
    std::cout << std::endl;

    std::string unencoded{"url encoded*string"};
    std::cout << "unencoded: " << unencoded << std::endl;
    std::cout << "encoded: " << encode(unencoded) << std::endl;

/*
    uint32_t c{' '};
    std::cout << "character: " << static_cast<char>(++c) << " (" << std::hex << c << ")" << std::endl;
    std::cout << "character: " << static_cast<char>(++c) << " (" << std::hex << c << ")" << std::endl;
    std::cout << "character: " << static_cast<char>(++c) << " (" << std::hex << c << ")" << std::endl;
*/

    return 0;
}
