/*
 * coproto-client_main.cpp
 *
 *  Created on: Sep 19, 2016
 *      Author: rpartridge
 */

#include <iostream>

#include <DriverImpl.hpp>

int main(int argc, char* argv[])
{
    std::cout << "entering main" << std::endl;
    DriverImpl driver;
    driver.start();
    std::cout << "exiting main" << std::endl;
    return 0;

}
