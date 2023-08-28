#include "mqtt/async_client.hpp"
#include <iostream>
#include <mqtt_client_cpp.hpp>

#include <boost/asio.hpp>

int main(int argc, char* argv[])
{
    std::cout << "start mqtt_cpp_test" << std::endl;

    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
        return 1;
    }

    boost::asio::io_context io_context;
    auto client = MQTT_NS::make_async_client(io_context, argv[1], argv[2], MQTT_NS::protocol_version::v5);


    io_context.run();
    std::cout << "end mqtt_cpp_test" << std::endl;
    return 0;
}

