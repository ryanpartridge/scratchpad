#include <iostream>
#include <sstream>

#include <boost/shared_ptr.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "Base.hpp"
#include "Foo.hpp"
#include "Bar.hpp"

int main(int argc, char* argv[])
{
    Foo foo1;
    Bar bar2;

    std::ostringstream out;
    boost::archive::text_oarchive oa(out);

    // save foo1 to the archive
    oa & foo1;

    out << std::endl;

    // save foo2 to the archive
    oa & bar2;

    // output the archive
    std::cout << "archive payload: " << out.str() << std::endl;

    //delete foo2;

    return 0;
}
