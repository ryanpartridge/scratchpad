/*
 * CommputeClient.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: rpartridge
 */

#include <iostream>
#include <string.h>

#include <ComputeService.h>
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
    boost::shared_ptr<TTransport> socket(new TSocket("/tmp/thrift-sample2"));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    ComputeServiceClient client(protocol);

    try
    {
        transport->open();
        int seqNum = 6;
        int fib = client.getFib(seqNum);
        std::cout << "Fibonacci of " << seqNum << ": " << fib << std::endl;

        int base = 6;
        int factorial = client.getFactorial(base);
        std::cout << "Factorial of " << base << ": " << factorial << std::endl;

        transport->close();
    }
    catch (TException& ex)
    {
        std::cerr << "ERROR: " << ex.what() << std::endl;
    }

    return 0;
}
