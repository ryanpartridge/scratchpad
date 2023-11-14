#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/ssl.h>

int main(int argc, char* argv[])
{
    std::cout << "starting ssl_verify" << std::endl;

    auto sslContext = boost::asio::ssl::context(boost::asio::ssl::context::tlsv12);
    auto nativeContext = sslContext.native_handle();
    auto verifyMode = SSL_CTX_get_verify_mode(nativeContext);

    std::cout << "default verify mode: ";
    switch (verifyMode)
    {
        case SSL_VERIFY_NONE:
            std::cout << "NONE";
            break;
        case SSL_VERIFY_PEER:
            std::cout << "PEER";
            break;
        default:
            std::cout << "OTHER";
            break;
    }
    std::cout << std::endl;

    sslContext.set_verify_mode(boost::asio::ssl::context::verify_peer);
    verifyMode = SSL_CTX_get_verify_mode(nativeContext);
    std::cout << "new verify mode: ";
    switch (verifyMode)
    {
        case SSL_VERIFY_NONE:
            std::cout << "NONE";
            break;
        case SSL_VERIFY_PEER:
            std::cout << "PEER";
            break;
        default:
            std::cout << "OTHER";
            break;
    }
    std::cout << std::endl;

    return 0;
}
