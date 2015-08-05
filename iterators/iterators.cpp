/*
 * iterators.cpp
 *
 *  Created on: Aug 5, 2015
 *      Author: rpartridge
 */

#include <set>
#include <iostream>
#include <utility>
#include <boost/foreach.hpp>


typedef std::multiset<double> DoubleMultiset;

int main(int argc, char* argv[])
{
    DoubleMultiset ms;
    ms.insert(0.0);
    ms.insert(2.5);
    ms.insert(0.1);
    ms.insert(0.0);
    ms.insert(10.2);
    ms.insert(3.7);
    ms.insert(0.0);
    ms.insert(6.9);

    std::cout.precision(1);
    std::cout << "original order" << std::fixed << std::endl;
    BOOST_FOREACH(double d, ms)
    {
        std::cout << d << std::endl;
    }
    std::cout << std::endl;

    std::cout << "forward iterator after 0.0" << std::endl;
    BOOST_FOREACH(double d, std::make_pair(ms.upper_bound(0.0), ms.end()))
    {
        std::cout << d << std::endl;
    }
    std::cout << std::endl;

    std::cout << "reverse iterator before 0.0" << std::endl;
    BOOST_FOREACH(double d, std::make_pair(ms.rbegin(), DoubleMultiset::reverse_iterator(ms.upper_bound(0.0))))
    {
        std::cout << d << std::endl;
    }
    std::cout << std::endl;
}
