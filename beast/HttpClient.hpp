#pragma once

#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/beast/http.hpp>

#include <HttpRequest.hpp>

namespace c4 {
namespace net {
namespace http {

class HttpResponse;

class HttpClient :
    public std::enable_shared_from_this<HttpClient>,
    boost::noncopyable
{
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
    void handleWrite(const boost::system::error_code& ec);

    boost::asio::io_context& io_context_;
    std::shared_ptr<handle_response_func_type> handleResponseFunc_;

    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
    std::unique_ptr<boost::beast::http::request<boost::beast::http::empty_body>> req_;
    HttpRequest request_;
};

}
}
}
