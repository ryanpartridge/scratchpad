#include <atomic>
#include <utility>
#include <fstream>
#include <tuple>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/http/file_body.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <memory.hpp>
#include <HttpClient.hpp>
#include <types.hpp>
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
    const auto& url = request.url();
    if (!url.isValid())
    {
        return boost::system::errc::make_error_code(boost::system::errc::invalid_argument);
    }

    if (!request.destination().empty() && !isFileValid(request.destination(), true))
    {
        return boost::system::errc::make_error_code(boost::system::errc::invalid_argument);
    }

    if (request.payloadIsFile() && (request.payload().empty() || !isFileValid(request.payload())))
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
    auto func = std::atomic_load_explicit(&handleResponseFunc_, std::memory_order_acquire);
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
        return;
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
        return;
    }

    // decide which kind of body to use based on the
    // method and presence of a request payload
    if (request_.payload().empty())
    {
        writeRequest<boost::beast::http::empty_body>(boost::beast::http::empty_body::value_type());
    }
    else if (request_.payloadIsFile())
    {
        boost::beast::http::file_body::value_type body;
        boost::system::error_code bodyError;
        body.open(request_.payload().c_str(), boost::beast::file_mode::scan, bodyError);

        if (bodyError)
        {
            invokeHandleResponse(HttpResponse(), bodyError);
        }
        else
        {
            writeRequest<boost::beast::http::file_body>(std::move(body));
        }
    }
    else
    {
        writeRequest<boost::beast::http::string_body>(
            boost::beast::http::string_body::value_type(request_.payload()));
    }
}

template<class Body>
void HttpClient::writeRequest(typename Body::value_type&& bodyArg)
{
    auto req = std::make_shared<boost::beast::http::request<Body>>(
            request_.method(),
            request_.url().path(),
            (boost::iequals(request_.httpVersion(), "1.0") ? 10 : 11),
            std::forward<typename Body::value_type>(bodyArg),
            request_.fields());

    req->set(boost::beast::http::field::host, request_.url().domain());
    req->set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req->prepare_payload();

    auto instance = shared_from_this();
    boost::beast::http::async_write(socket_,
        *req,
        [instance, req](const boost::system::error_code& ec, std::size_t)
            {
                instance->handleWrite(ec);
            }
    );
}

void HttpClient::handleWrite(const boost::system::error_code& ec)
{
    buffer_.consume(buffer_.size());

    if (ec)
    {
        invokeHandleResponse(HttpResponse(), ec);
        return;
    }

    if (request_.destination().empty())
    {
        readResponse<boost::beast::http::string_body>(boost::beast::http::string_body::value_type());
    }
    else
    {
        boost::beast::http::file_body::value_type body;
        boost::system::error_code bodyError;
        body.open(request_.destination().c_str(), boost::beast::file_mode::write, bodyError);

        if (bodyError)
        {
            invokeHandleResponse(HttpResponse(), bodyError);
        }
        else
        {
            readResponse<boost::beast::http::file_body>(std::move(body));
        }
    }
}

template<class Body>
void HttpClient::readResponse(typename Body::value_type&& bodyArg)
{
    auto res = std::make_shared<boost::beast::http::response<Body>>(
            std::piecewise_construct,
            std::make_tuple(std::move(bodyArg)));

    auto instance = shared_from_this();
    boost::beast::http::async_read(socket_,
        buffer_,
        *res,
        [instance, res](const boost::system::error_code& err, std::size_t)
            {
                instance->handleRead(res, err);
            }
    );
}

template<class Body>
void HttpClient::handleRead(std::shared_ptr<boost::beast::http::response<Body>> res, const boost::system::error_code & ec)
{
    buffer_.consume(buffer_.size());

    if (ec)
    {
        invokeHandleResponse(HttpResponse(), ec);
        return;
    }

    HttpResponse response(res->result());
    response.fields(*res);
    handleBody<Body>(res->body(), response);

    invokeHandleResponse(response, ec);
}

template<class Body>
void HttpClient::handleBody(const typename Body::value_type& /* body */, HttpResponse& /* response */)
{
    // don't do anything on an empty or unknown body type
}

template<>
void HttpClient::handleBody<boost::beast::http::string_body>(const boost::beast::http::string_body::value_type& body, HttpResponse& response)
{
    response.payload(body);
}

template<>
void HttpClient::handleBody<boost::beast::http::file_body>(const boost::beast::http::file_body::value_type& body, HttpResponse& response)
{
    response.destination(request_.destination());
}

void HttpClient::invokeHandleResponse(const HttpResponse& response, const boost::system::error_code& ec)
{
    request_ = HttpRequest();
    auto func = std::atomic_load_explicit(&handleResponseFunc_, std::memory_order_acquire);
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

bool HttpClient::isFileValid(const std::string& path, bool writeable /* = false */)
{
    boost::system::error_code ec;
    boost::filesystem::path filePath(path);

    if (boost::filesystem::exists(filePath, ec))
    {
        ec = boost::system::error_code();
        if (boost::filesystem::is_directory(filePath, ec))
        {
            return false;
        }

        if (writeable)
        {
            ec = boost::system::error_code();
            boost::filesystem::remove(path, ec);
            if (ec)
            {
                return false;
            }
        }
    }

    std::unique_ptr<std::fstream> stream;
    auto flags = std::fstream::in;
    if (writeable)
    {
        flags = std::fstream::out;
    }
    stream = std::make_unique<std::fstream>(filePath.string(), flags);
    if (!stream->good())
    {
        return false;
    }

    return true;
}

}
}
}
