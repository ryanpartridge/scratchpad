/*
 * MessageClient.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: rpartridge
 */

#include <iostream>
#include <string.h>

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
    boost::shared_ptr<TTransport> socket(new TSocket("/tmp/thrift-sample"));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    MessageServiceClient client(protocol);

    char* buffer = new char[1024];
    memset(buffer, 1024, 'a');

    std::string contents = "";

    try
    {
        transport->open();

        for (int i = 0; i < 15; ++i)
        {
            //std::cout << "putting entry: " << i << std::endl;
            Message msg1;
            msg1.key = i;
            if (i == 0)
            {
                contents = std::string(buffer, 1024);
            }
            else
            {
                contents += contents;
            }
            std::cout << "message size: " << contents.size() << std::endl;
            msg1.value = contents;
            client.setMessage(msg1);

            Message msg2;
            //std::cout << "getting entry: " << i << std::endl;
            client.getMessage(msg2, i);
            if (msg2.value != contents)
            {
                std::cerr << "fetched value for " << i << " does not match!" << std::endl;
                break;
            }
            //std::cout << "success" << std::endl;
        }
        transport->close();
    }
    catch (TException& ex)
    {
        std::cerr << "ERROR: " << ex.what() << std::endl;
    }

    delete buffer;

    return 0;
}
