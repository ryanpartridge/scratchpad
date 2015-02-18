/*
 * director.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */
#include <iostream>

#include <DirectorServer.hpp>
#include <DriverClient.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

int main(int argc, char* argv[])
{
    std::cout << "entering main" << std::endl;
    boost::filesystem::path serverPath("/tmp/director_rpc");
    boost::filesystem::path clientPath("/tmp/driver_rpc");
    boost::system::error_code ec;
    if (boost::filesystem::exists(serverPath, ec))
    {
        boost::filesystem::remove(serverPath);
    }
    boost::shared_ptr<c4::DirectorServer> server = boost::make_shared<c4::DirectorServer>(serverPath.string(), clientPath.string());
    boost::shared_ptr<c4::DriverClient> client = boost::make_shared<c4::DriverClient>(clientPath.string());

    std::cout << "starting server" << std::endl;
    server->start();

    boost::this_thread::sleep_for(boost::chrono::seconds(1));

    std::cout << "initializing client" << std::endl;
    client->initialize();

    std::string response = client->doAction(5);
    std::cout << "driver reponse: " << response << std::endl;

    std::cout << "stopping server" << std::endl;
    server->stop();
    if (boost::filesystem::exists(serverPath, ec))
    {
        boost::filesystem::remove(serverPath);
    }
    std::cout << "exiting main" << std::endl;
}
