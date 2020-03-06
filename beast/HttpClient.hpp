#pragma once

#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#if BOOST_VERSION >= 107000
#include <boost/beast/core/basic_stream.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>
#else
#include <boost/asio/ssl/stream.hpp>
#endif

#include <HttpRequest.hpp>

namespace c4 {
namespace net {
namespace http {

class HttpResponse;

class HttpClient :
    public std::enable_shared_from_this<HttpClient>,
    boost::noncopyable
{
#if BOOST_VERSION >= 107000
    using stream_type = boost::beast::basic_stream<boost::beast::net::ip::tcp, boost::beast::net::executor, boost::beast::simple_rate_policy>;
    using ssl_stream_type = boost::beast::ssl_stream<stream_type>;
    using ssl_context_type = boost::beast::net::ssl::context;
#else
    using stream_type = boost::asio::ip::tcp::socket;
    using ssl_stream_type = boost::asio::ssl::stream<stream_type>;
    using ssl_context_type = boost::asio::ssl::context;
#endif

public:
    using handle_response_func_type = boost::function<void(const HttpResponse&, const boost::system::error_code&)>;

    explicit HttpClient(boost::asio::io_context& io_context);
    HttpClient(boost::asio::io_context& io_context, handle_response_func_type handleResponseFunc);

    virtual ~HttpClient() noexcept;

    void cancel();
    void close();
    boost::system::error_code asyncRequest(HttpRequest const& request);

    handle_response_func_type handleResponseFunc() const;
    void handleResponseFunc(handle_response_func_type func);
    void clearHandleResponseFunc();

private:
    void invokeHandleResponse(const HttpResponse& response, const boost::system::error_code& ec);
    void handleResolve(const boost::system::error_code& ec, const boost::asio::ip::tcp::resolver::results_type& endpoints);
    void handleConnect(const boost::system::error_code& ec);
    void handleSSLConnect(const boost::system::error_code& ec);
    void handleHandshake(const boost::system::error_code& ec);
    void writeRequest();
    template<class Body> void writeRequest(typename Body::value_type&& bodyArg);
    void handleWrite(const boost::system::error_code& ec);
    template<class Body> void readResponse(typename Body::value_type&& bodyArg);
    template<class Body> void handleRead(std::shared_ptr<boost::beast::http::response<Body>> res, const boost::system::error_code& ec);
    template<class Body> void handleBody(const typename Body::value_type& body, HttpResponse& response);

    bool isFileValid(const std::string& path, bool writeable = false);

    boost::asio::io_context& io_context_;
    std::shared_ptr<handle_response_func_type> handleResponseFunc_;

    boost::asio::ip::tcp::resolver resolver_;
    std::unique_ptr<stream_type> socket_;
    std::unique_ptr<ssl_context_type> sslContext_;
    std::unique_ptr<ssl_stream_type> sslSocket_;
    HttpRequest request_;
    boost::beast::flat_buffer buffer_;
};

}
}
}
