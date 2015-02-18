/*
 * DirectorClient.hpp
 *
 *  Created on: Feb 9, 2015
 *      Author: rpartridge
 */

#ifndef DRIVERCLIENT_HPP_
#define DRIVERCLIENT_HPP_

#include <DirectorService.h>
#include <thrift/transport/TSocket.h>
#include <boost/shared_ptr.hpp>

namespace c4
{
    class DirectorClient
    {
    public:
        explicit DirectorClient(const std::string& socketPath);
        virtual ~DirectorClient();

        void initialize();
        std::string getValue(const int id);

    private:
        std::string path_;
        boost::shared_ptr<apache::thrift::transport::TSocket> socket_;
        boost::shared_ptr<apache::thrift::transport::TTransport> transport_;
        boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol_;
        boost::shared_ptr<DirectorServiceClient> client_;
    };

} /* namespace c4 */

#endif /* DRIVERCLIENT_HPP_ */
