#include <iostream>
#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>

using StringContainer =
    boost::multi_index::multi_index_container<
        std::string,
        boost::multi_index::indexed_by<
            boost::multi_index::ordered_non_unique<
                boost::multi_index::identity<
                    std::string
                >
            >
        >
    >;

bool returnBoolean(bool value)
{
    std::cout << "returning value: " << std::boolalpha << value << std::endl;
    return value;
}

int main(int argc, char* argv[])
{
    std::cout << "index replace test" << std::endl;

    StringContainer strings;
    strings.insert(std::string("one"));
    strings.insert(std::string("two"));
    strings.insert(std::string("three"));
    auto inserted = strings.insert(std::string("three"));
    std::cout << std::endl << "inserted: " << std::boolalpha << inserted.second << std::endl;

    std::cout << std::endl << "initial set:" << std::endl;
    for (const auto s: strings)
    {
        std::cout << "value: " << s << std::endl;
    }

    auto replaced = false;
    auto two = strings.find(std::string("two"));
    if (two != strings.end())
    {
        replaced = strings.replace(two, std::string("one"));
    }

    std::cout << std::endl << "after replace (" << std::boolalpha << replaced << "):" << std::endl;
    for (const auto s: strings)
    {
        std::cout << "value: " << s << std::endl;
    }

    auto iters = strings.equal_range(std::string("seven"));
    if (iters.first != strings.end())
    {
        strings.erase(iters.first, iters.second);
    }

    std::cout << std::endl << "after erase" << std::endl;
    for (const auto s: strings)
    {
        std::cout << "value: " << s << std::endl;
    }

    bool modified = true;
    std::cout << std::endl << "initial modified value: " << std::boolalpha << modified << std::endl;
    modified |= returnBoolean(false);
    std::cout << "new modified value: " << std::boolalpha << modified << std::endl;

    return 0;
}
