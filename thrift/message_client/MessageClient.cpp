/*
 * MessageClient.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: rpartridge
 */

#include <iostream>

#include <MessageService.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using boost::shared_ptr;

using namespace ::sample;

int main(int argc, char* argv[])
{
    boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    MessageServiceClient client(protocol);

    try
    {
        transport->open();
        std::cout << "putting 0:hello" << std::endl;
        Message msg1;
        msg1.key = 0;
        msg1.value = "hello";
        client.setMessage(msg1);

        Message msg2;
        std::cout << "getting 0" << std::endl;
        client.getMessage(msg2, 0);
        std::cout << "key: " << msg2.key << " value: " << msg2.value << std::endl;
        transport->close();
    }
    catch (TException& ex)
    {
        std::cerr << "ERROR: " << ex.what() << std::endl;
    }
    return 0;
}
