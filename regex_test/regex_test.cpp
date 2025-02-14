#include <iostream>
#include <regex>

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

    std::cout << std::endl;

    boost::regex depRegex("^(.+)[[:space:]]+\\([[:space:]]*([>=<])+[[:space:]]+([^[:space:]]+)[[:space:]]*\\).*$");
    std::string dependency = "part-a (    = 3.5.1   )";
    if (boost::regex_match(dependency, matches, depRegex))
    {
        std::cout << "matched the suffix" << std::endl;
        std::cout << "found " << matches.size() << " matches" << std::endl;
        for (const auto& match : matches)
        {
            const auto& value = match.str();
            if (!value.empty())
            {
                std::cout << "match value: ***" << match.str() << "***" << std::endl;
            }
        }
    }
    else
    {
        std::cout << "did not match the dependency" << std::endl;
    }

    std::regex mmpRegex = std::regex("[[:digit:]]+\\.[[:digit:]]+\\.[[:digit:]]+");
    if (std::regex_match("3.10.2", mmpRegex))
    {
        std::cout << "target version matches the std::regex" << std::endl;
    }
    else
    {
        std::cout << "target version does not match the std::regex" << std::endl;
    }

    return 0;
}
