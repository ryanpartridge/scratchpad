/*
 * coproto-client_main.cpp
 *
 *  Created on: Sep 19, 2016
 *      Author: rpartridge
 */

#include <DriverImpl.hpp>

int main(int argc, char* argv[])
{
    DriverImpl driver;
    driver.start();
    return 0;
}
