/*
 * split_join.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: ryan
 */
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/range/algorithm/replace_if.hpp>
#include <boost/regex.hpp>

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

void replaceDisallowedChars(string& buffer)
{
    cout << "input: " << buffer << endl;
    boost::algorithm::replace_all(buffer, "][", "?");
    string disallowed = "\"\'\\!@#$%*+={}|[]:;<>?/";
    boost::replace_if(buffer, boost::algorithm::is_any_of(disallowed), '_');
    cout << "modified: " << buffer << endl;
}

void replaceDisallowedCharsCompact(string& buffer)
{
    cout << "input: " << buffer << endl;
    boost::algorithm::replace_all(buffer, "][", "?");
    string disallowed = "\"\'\\!@#$%*+={}|[]:;<>?/";
    boost::algorithm::replace_all_regex(buffer, boost::regex("[\"\'\\\\!@#$%*+={}|[\\]:;<>?/]+"), string("_"));
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

    string sampleWithNewlines("hello\nworld\r\nthis is a new\rday for\n\n\n\n\nus\r\n\r\n");
    cout << "original string: ***" << sampleWithNewlines << "***" << endl;
    cout << "newlines replaced: ***" << boost::algorithm::replace_all_regex_copy(sampleWithNewlines, boost::regex("[\\r\\n]+"), string(" ")) << "***" << endl;
    cout << endl;

    string cramfsManyLines("TEXT hello\nTEXT another line\nSUCCESS ok\n");
    cout << "original string: ***" << cramfsManyLines << "***" << endl;
    parts.clear();
    boost::algorithm::trim(cramfsManyLines);
    boost::algorithm::split(parts, cramfsManyLines, boost::algorithm::is_any_of("\n"), boost::algorithm::token_compress_on);
    printStringVec(parts);

    string hostsLine("    127.0.0.1  \t  director    ");
    boost::regex pattern("^[[:space:]]*[[:digit:]]{1,3}\\.[[:digit:]]{1,3}\\.[[:digit:]]{1,3}\\.[[:digit:]]{1,3}[[:space:]]+director[[:space:]]*$");
    bool matches = boost::regex_match(hostsLine, pattern);
    cout << "line matches: " << (matches ? "true" : "false") << endl;

    string updateUrl("http://services-beta2.control4.com/Updates2x-earlyaccess/v2_0/Updates.asmx ");
    boost::regex urlPattern("^https?://[^ \t\n]+\\.control4.com/[^ \t\n]+$");
    matches = boost::regex_match(updateUrl, urlPattern);
    cout << "url matches: " << (matches ? "true" : "false") << endl;

    cout << endl;
    string disallowedNameChars("Some ? name \\ with \" invalid start ___ end ][@$# characters ] %#$@{}  \"\'\\!@#$%*+={}|[]:;<>?/");
    replaceDisallowedChars(disallowedNameChars);

    cout << endl;
    string disallowedNameCharsCompact("Some ? name \\ with \" invalid start ___ end ][@$# characters ] %#$@{}  \"\'\\!@#$%*+={}|[]:;<>?/");
    replaceDisallowedCharsCompact(disallowedNameCharsCompact);

    return 0;
}
