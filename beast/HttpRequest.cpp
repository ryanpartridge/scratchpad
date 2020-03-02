#include <HttpRequest.hpp>

namespace c4 {
namespace net {
namespace http {

HttpRequest::HttpRequest() :
    method_(HttpMethod::get),
    redirects_(5),
    timeout_(120),
    payloadIsFile_(false)
{
}

HttpRequest::HttpRequest(const Url& url) :
    method_(HttpMethod::get),
    url_(url),
    redirects_(5),
    timeout_(120),
    payloadIsFile_(false)
{
}

HttpRequest::HttpRequest(HttpMethod method) :
    method_(method),
    redirects_(5),
    timeout_(120),
    payloadIsFile_(false)
{
}

HttpRequest::HttpRequest(HttpMethod method, const Url& url) :
    method_(method),
    url_(url),
    redirects_(5),
    timeout_(120),
    payloadIsFile_(false)
{
}

HttpRequest::~HttpRequest() noexcept
{
}

void HttpRequest::method(HttpMethod method)
{
    method_ = method;
}

void HttpRequest::url(const Url& url)
{
    url_ = url;
}

boost::system::error_code HttpRequest::url(const std::string& urlString)
{
    boost::system::error_code ec;
    url_ = Url::createUrl(urlString, ec);
    return ec;
}

void HttpRequest::redirects(std::uint16_t redirects)
{
    redirects_ = redirects;
}

void HttpRequest::timeout(std::uint16_t timeout)
{
    timeout_ = timeout;
}

void HttpRequest::destination(const std::string& destination)
{
    destination_ = destination;
}

void HttpRequest::payload(const std::string& payload)
{
    payload_ = payload;
}

void HttpRequest::payloadIsFile(bool payloadIsFile)
{
    payloadIsFile_ = payloadIsFile;
}

std::string HttpRequest::httpVersion() const
{
    return getField(HttpField::version);
}

void HttpRequest::httpVersion(const std::string& version)
{
    setField(HttpField::version, version);
}

std::string HttpRequest::host() const
{
    return getField(HttpField::host);
}

void HttpRequest::host(const std::string& host)
{
    setField(HttpField::host, host);
}

std::string HttpRequest::accept() const
{
    return getField(HttpField::accept);
}

void HttpRequest::accept(const std::string& accept)
{
    setField(HttpField::accept, accept);
}

std::string HttpRequest::range() const
{
    return getField(HttpField::range);
}

void HttpRequest::range(const std::string& range)
{
    setField(HttpField::range, range);
}

std::string HttpRequest::cookie() const
{
    return getField(HttpField::cookie);
}

void HttpRequest::cookie(const std::string& cookie)
{
    setField(HttpField::cookie, cookie);
}

std::string HttpRequest::referer() const
{
    return getField(HttpField::referer);
}

void HttpRequest::referer(const std::string& referer)
{
    setField(HttpField::referer, referer);
}

std::string HttpRequest::userAgent() const
{
    return getField(HttpField::user_agent);
}

void HttpRequest::userAgent(const std::string& userAgent)
{
    setField(HttpField::user_agent, userAgent);
}

std::string HttpRequest::contentType() const
{
    return getField(HttpField::content_type);
}

void HttpRequest::contentType(const std::string& contentType)
{
    setField(HttpField::content_type, contentType);
}

std::string HttpRequest::contentLength() const
{
    return getField(HttpField::content_length);
}

void HttpRequest::contentLength(const std::string& contentLength)
{
    setField(HttpField::content_length, contentLength);
}

std::string HttpRequest::contentRange() const
{
    return getField(HttpField::content_range);
}

void HttpRequest::contentRange(const std::string& contentRange)
{
    setField(HttpField::content_range, contentRange);
}

std::string HttpRequest::connection() const
{
    return getField(HttpField::connection);
}

void HttpRequest::connection(const std::string& connection)
{
    setField(HttpField::connection, connection);
}

std::string HttpRequest::proxyConnection() const
{
    return getField(HttpField::proxy_connection);
}

void HttpRequest::proxyConnection(const std::string& proxyConnection)
{
    setField(HttpField::proxy_connection, proxyConnection);
}

std::string HttpRequest::acceptEncoding() const
{
    return getField(HttpField::accept_encoding);
}

void HttpRequest::acceptEncoding(const std::string& acceptEncoding)
{
    setField(HttpField::accept_encoding, acceptEncoding);
}

std::string HttpRequest::transferEncoding() const
{
    return getField(HttpField::transfer_encoding);
}

void HttpRequest::transferEncoding(const std::string& transferEncoding)
{
    setField(HttpField::transfer_encoding, transferEncoding);
}

std::string HttpRequest::contentEncoding() const
{
    return getField(HttpField::content_encoding);
}

void HttpRequest::contentEncoding(const std::string& contentEncoding)
{
    setField(HttpField::content_encoding, contentEncoding);
}

void HttpRequest::fields(const HttpFields& fields)
{
    fields_ = fields;
}

std::string HttpRequest::getField(const std::string& field) const
{
    const auto it = fields_.find(field);
    if (it != fields_.end())
    {
        return it->value().to_string();
    }
    return std::string();
}

std::string HttpRequest::getField(HttpField field) const
{
    // the underlying implementation of find turns the field
    // into a string anyway, so might as well do it here
    return getField(toString(field));
}

void HttpRequest::setField(const std::string& field, const std::string& value)
{
    fields_.set(field, value);
}

void HttpRequest::setField(HttpField field, const std::string& value)
{
    fields_.set(field, value);
}

}
}
}
