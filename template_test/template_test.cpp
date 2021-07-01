/*
 * template_test.cpp
 *
 *  Created on: Aug 7, 2013
 *      Author: rpartridge
 */
#include <memory>

#include <Concrete.hpp>

using namespace std;

int main(int argc, char* argv[])
{
    auto concrete = std::make_shared<Concrete>();
    return 0;
}
