/*
 * callback_chaining.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>

typedef boost::function<void(int, std::string)> LowLevelCallback;

int main(int argc, char* argv[])
{
    boost::asio::io_service io_service;
    std::cout << "[main] calling run" << std::endl;
    io_service.run();
    std::cout << "[main] run complete" << std::endl;
    return 0;
}
