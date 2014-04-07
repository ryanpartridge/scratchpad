/*
 * dir_iterator.cpp
 *
 *  Created on: Apr 7, 2014
 *      Author: ryan
 */
#include <string>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

using namespace std;

int main(int argc, char* argv[])
{
    boost::filesystem::directory_iterator it("/tmp"), end;
    BOOST_FOREACH(const boost::filesystem::path& p, std::make_pair(it, end))
    {
        cout << p.filename().string() << endl;
        if (p.filename().string()[0] == 'h')
        {
            break;
        }
    }

    return 0;
}
