/*
 * DriverClient.hpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */

#ifndef DRIVERCLIENT_HPP_
#define DRIVERCLIENT_HPP_

#include <DriverService.h>
#include <thrift/transport/TSocket.h>
#include <boost/shared_ptr.hpp>

namespace c4
{
    class DriverClient
    {
    public:
        explicit DriverClient(const std::string& socketPath);
        virtual ~DriverClient();

        void initialize();
        std::string doAction(const int driverId);

    private:
        std::string path_;
        boost::shared_ptr<apache::thrift::transport::TSocket> socket_;
        boost::shared_ptr<apache::thrift::transport::TTransport> transport_;
        boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol_;
        boost::shared_ptr<DriverServiceClient> client_;
    };

} /* namespace c4 */

#endif /* DRIVERCLIENT_HPP_ */
