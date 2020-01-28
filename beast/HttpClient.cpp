#include <atomic>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/algorithm/string.hpp>

#include <HttpClient.hpp>
#include <HttpRequest.hpp>
#include <HttpResponse.hpp>


namespace c4 {
namespace net {
namespace http {

HttpClient::HttpClient(boost::asio::io_context& io_context) :
    io_context_(io_context),
    resolver_(io_context),
    socket_(io_context)
{
}

HttpClient::HttpClient(boost::asio::io_context& io_context, handle_response_func_type handleResponseFunc) :
    io_context_(io_context),
    handleResponseFunc_(std::make_shared<handle_response_func_type>(handleResponseFunc)),
    resolver_(io_context),
    socket_(io_context)
{
}

HttpClient::~HttpClient() noexcept
{
    cancel();
}

void HttpClient::cancel()
{
    // TODO: implement
}

void HttpClient::close()
{
    // TODO: implement
}

boost::system::error_code HttpClient::asyncRequest(HttpRequest const& request)
{
    boost::system::error_code ec;
    const Url& url = request.url();
    if (!url.isValid())
    {
        return boost::system::errc::make_error_code(boost::system::errc::invalid_argument);
    }

    request_ = request;

    // first step is to resolve the domain
    resolver_.async_resolve(url.domain(),
            boost::lexical_cast<std::string>(url.port()),
            boost::bind(&HttpClient::handleResolve,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::results));
    return ec;
}

HttpClient::handle_response_func_type HttpClient::handleResponseFunc() const
{
    std::shared_ptr<handle_response_func_type> func = std::atomic_load_explicit(&handleResponseFunc_, std::memory_order_acquire);
    if (func)
        return *func;
    else
        return handle_response_func_type();
}

void HttpClient::handleResponseFunc(handle_response_func_type func)
{
    std::atomic_store_explicit(&handleResponseFunc_, std::make_shared<handle_response_func_type>(func),
        std::memory_order_release);
}

void HttpClient::clearHandleResponseFunc()
{
    std::atomic_store_explicit(&handleResponseFunc_, std::shared_ptr<handle_response_func_type>(),
        std::memory_order_release);
}

void HttpClient::handleResolve(boost::system::error_code const& ec, boost::asio::ip::tcp::resolver::results_type const& endpoints)
{
    if (ec)
    {
        invokeHandleResponse(HttpResponse(), ec);
    }

    boost::asio::async_connect(socket_,
            endpoints,
            boost::bind(&HttpClient::handleConnect,
                shared_from_this(),
                boost::asio::placeholders::error));
}

void HttpClient::handleConnect(const boost::system::error_code& ec)
{
    if (ec)
    {
        invokeHandleResponse(HttpResponse(), ec);
    }

    std::shared_ptr<boost::beast::http::request<boost::beast::http::empty_body>> req = std::make_shared<boost::beast::http::request<boost::beast::http::empty_body>>();
    req->version((boost::iequals(request_.httpVersion(), "1.0") ? 10 : 11));
    req->method(request_.method());
    req->target(request_.url().path());
    req->set(boost::beast::http::field::host, request_.url().domain());
    req->set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    auto instance = shared_from_this();
    boost::beast::http::async_write(socket_,
        *req,
        [instance, req](const boost::system::error_code& ec, std::size_t)
            {
                instance->handleWrite(ec);
            }
    );

/*
    boost::beast::http::async_write(socket_,
            req,
            boost::bind(&HttpClient::handleWrite,
                shared_from_this(),
                boost::asio::placeholders::error));
*/
}

void HttpClient::handleWrite(const boost::system::error_code& ec)
{
    if (ec)
    {
        invokeHandleResponse(HttpResponse(), ec);
    }

    invokeHandleResponse(HttpResponse(), ec);
}

void HttpClient::invokeHandleResponse(const HttpResponse& response, const boost::system::error_code& ec)
{
    request_ = HttpRequest();
    std::shared_ptr<handle_response_func_type> func = std::atomic_load_explicit(&handleResponseFunc_, std::memory_order_acquire);
    if (func)
    {
        try
        {
            (*func)(response, ec);
        }
        catch (const boost::bad_function_call&)
        {
            clearHandleResponseFunc();
        }
    }
}

}
}
}
