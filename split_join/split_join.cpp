/*
 * split_join.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: ryan
 */
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

typedef std::vector<std::string> StringVec;

using namespace std;

void printStringVec(const StringVec& strings)
{
    cout << "number of strings: " << strings.size() << endl;
	StringVec::const_iterator i = strings.begin(), end = strings.end();
    for (; i != strings.end(); ++i)
    {
        cout << "entry: ***" << *i << "***" << endl;
    }
}

void removeLineEnding(string& buffer)
{
	cout << "input: " << buffer << endl;
	boost::algorithm::erase_all(buffer, "\n");
	cout << "modified: " << buffer << endl;
}

void compressSpaces(string& buffer)
{
	cout << "input: " << buffer << endl;
	boost::algorithm::erase_all(buffer, "\n");
	cout << "modified: " << buffer << endl;
}

StringVec splitOnPipe(string& buffer)
{
	StringVec parts;
	boost::algorithm::split(parts, buffer, boost::algorithm::is_any_of("|"), boost::algorithm::token_compress_on);
	return parts;
}

int main(int argc, char* argv[])
{
    string input = "/,";
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
            StringVec::iterator removedIt = remove(parts.begin(),
                parts.end(), "");
            parts.resize(distance(parts.begin(), removedIt));
        }
    }

    printStringVec(parts);
    cout << endl;

    string lineEndings = "this line \nhas embedded line\n\n\n endings that should be\n removed\n\n\n\n";
    removeLineEnding(lineEndings);
    cout << endl;

    string dpkgEntry = "ii |dpkg|1.16.10-1|DPKG Packaging Utility";
    parts = splitOnPipe(dpkgEntry);
    printStringVec(parts);
    cout << endl;

    dpkgEntry = "ii |netsnmp|5.7.1-1|\n This package provides net-snmp.";
    parts = splitOnPipe(dpkgEntry);
    printStringVec(parts);
    cout << endl;

    dpkgEntry = "ii |netsnmp|5.7.1-1|";
    parts = splitOnPipe(dpkgEntry);
    printStringVec(parts);
    cout << endl;

    dpkgEntry = " some random text on a line";
    parts = splitOnPipe(dpkgEntry);
    printStringVec(parts);
    cout << endl;

    dpkgEntry = "";
    parts = splitOnPipe(dpkgEntry);
    printStringVec(parts);
    cout << endl;
    return 0;
}
