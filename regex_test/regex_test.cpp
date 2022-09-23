#include <iostream>

#include <boost/algorithm/string/regex.hpp>

int main(int argc, char* argv[])
{
    std::cout << "starting regex_test" << std::endl;

    boost::regex suffixRegex("^.+(-r[[:digit:]]+(\\.[[:digit:]]+)?)$");
    boost::smatch matches;
    std::string version = "3.4.0.123456-res-r123.456";
    if (boost::regex_match(version, matches, suffixRegex))
    {
        std::cout << "matched the suffix" << std::endl;
        std::cout << "found " << matches.size() << " matches" << std::endl;
        for (const auto& match : matches)
        {
            const auto& value = match.str();
            if (!value.empty())
            {
                std::cout << "match value: " << match.str() << std::endl;
            }
        }
        const auto& suffix = matches[1].str();
        version = version.substr(0, version.length() - suffix.length());
        std::cout << "modified version: " << version << std::endl;
    }
    else
    {
        std::cout << "did not match the suffix" << std::endl;
    }

    return 0;
}
