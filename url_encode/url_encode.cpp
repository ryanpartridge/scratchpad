#include <iostream>
#include <sstream>
#include <cstdint>
#include <string>

std::string encode(std::string const& input, std::string const& encodeChars)
{
    auto index = size_t{0};
    auto lastIndex = index;
    auto length = input.size();
    std::ostringstream encoded;

    while (lastIndex < length)
    {
        index = input.find_first_of(encodeChars, lastIndex);
        if (index != std::string::npos)
        {
            encoded.write(input.c_str() + lastIndex, index - lastIndex);
            auto c = input.at(index);
            encoded.put('%');
            if (c < 10)
            {
                encoded.put('0');
            }
            encoded << std::uppercase << std::hex << static_cast<int>(c);
            lastIndex = index + 1;
        }
        else
        {
            encoded << (input.c_str() + lastIndex);
            lastIndex = index;
        }
    }

    return encoded.str();
}

std::string decode(std::string const& input)
{
    auto index = size_t(0);
    auto lastIndex = index;
    auto length = input.size();
    std::ostringstream decoded;

    while (lastIndex < length)
    {
        index = input.find('%', lastIndex);
        if (index != std::string::npos)
        {
            decoded.write(input.c_str() + lastIndex, index - lastIndex);
            ++index;
            if ((index + 1) < length)
            {
                auto value = input.substr(index, 2);
                try
                {
                    size_t badIndex = 0;
                    int code = std::stoi(value, &badIndex, 16);
                    if (badIndex == value.size())
                    {
                        decoded.put(static_cast<signed char>(code));
                    }
                    else
                    {
                        return decoded.str();
                    }
                }
                catch(std::exception const& ex)
                {
                    return decoded.str();
                }
                lastIndex = index + 2;
            }
            else
            {
                return decoded.str();
            }
        }
        else
        {
            decoded << (input.c_str() + lastIndex);
            lastIndex = index;
        }
    }

    return decoded.str();
}

int main(int argc, char* argv[])
{
    std::cout << "url_encode program" << std::endl;

    std::string encoded{"%20"};
    std::cout << "encoded: " << encoded << std::endl;
    std::cout << "decoded: " << decode(encoded) << std::endl;
    std::cout << std::endl;
    return 0;
}
