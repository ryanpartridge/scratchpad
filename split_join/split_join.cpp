/*
 * split_join.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: ryan
 */
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>

typedef std::vector<std::string> StringVec;

int main(int argc, char* argv[])
{
    std::string input = "/,";
    bool allowEmpty = false;

    StringVec parts;

    if (!input.empty())
    {
        boost::algorithm::split(parts, input,
            boost::algorithm::is_any_of("/"),
            (allowEmpty ?
                          boost::algorithm::token_compress_off :
                          boost::algorithm::token_compress_on));

        if (!allowEmpty)
        {
            StringVec::iterator removedIt = std::remove(parts.begin(),
                parts.end(), "");
            parts.resize(std::distance(parts.begin(), removedIt));
        }
    }

    std::cout << "number of tokens: " << parts.size() << std::endl;
    for (StringVec::const_iterator i = parts.begin(); i != parts.end();
        ++i)
    {
        std::cout << "entry: ***" << *i << "***" << std::endl;
    }

    return 0;
}
