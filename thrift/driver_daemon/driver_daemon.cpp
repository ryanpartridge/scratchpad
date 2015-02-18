/*
 * driver_daemon.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */
#include <iostream>

#include <DriverServer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

int main(int argc, char* argv[])
{
    std::cout << "entering main" << std::endl;
    boost::filesystem::path serverPath("/tmp/driver_rpc");
    boost::filesystem::path clientPath("/tmp/director_rpc");
    boost::system::error_code ec;
    if (boost::filesystem::exists(serverPath, ec))
    {
        boost::filesystem::remove(serverPath);
    }
    boost::shared_ptr<c4::DriverServer> server = boost::make_shared<c4::DriverServer>(serverPath.string(), clientPath.string());

    std::cout << "starting server" << std::endl;
    server->start();

    boost::this_thread::sleep_for(boost::chrono::seconds(20));

    std::cout << "stopping server" << std::endl;
    server->stop();
    if (boost::filesystem::exists(serverPath, ec))
    {
        boost::filesystem::remove(serverPath);
    }
    std::cout << "exiting main" << std::endl;
}
