#pragma once

#include <cstdint>
#include <types.hpp>
#include <Url.hpp>

namespace c4 {
namespace net {
namespace http {

class Url;

class HttpRequest
{
public:
    HttpRequest();
    explicit HttpRequest(const Url& url);
    explicit HttpRequest(HttpMethod method);
    HttpRequest(HttpMethod method, const Url& url);
    HttpRequest(const HttpRequest&) = default;

    HttpRequest& operator=(const HttpRequest&) = default;

    virtual ~HttpRequest() noexcept;

    HttpMethod method() const noexcept { return method_; };
    void method(HttpMethod method);

    const Url& url() const { return url_; };
    void url(const Url& url);
    boost::system::error_code url(const std::string& urlString);

    std::uint16_t redirects() const noexcept { return redirects_; };
    void redirects(std::uint16_t redirects);

    std::uint16_t timeout() const noexcept { return timeout_; };
    void timeout(std::uint16_t timeout);

    const std::string& destination() const { return destination_; };
    void destination(const std::string& destination);

    const HttpFields& fields() const { return fields_; };
    void fields(const HttpFields& fields);

    // these are convenience wrappers around getField and setField
    std::string httpVersion() const;
    void httpVersion(const std::string& version);
    // payload -- not sure how to handle this yet
    std::string host() const;
    void host(const std::string& host);
    std::string accept() const;
    void accept(const std::string& accept);
    std::string range() const;
    void range(const std::string& range);
    std::string cookie() const;
    void cookie(const std::string& cookie);
    std::string referer() const;
    void referer(const std::string& referer);
    std::string userAgent() const;
    void userAgent(const std::string& userAgent);
    std::string contentType() const;
    void contentType(const std::string& contentType);
    std::string contentLength() const;
    void contentLength(const std::string& contentLength);
    std::string contentRange() const;
    void contentRange(const std::string& contentRange);
    std::string connection() const;
    void connection(const std::string& connection);
    std::string proxyConnection() const;
    void proxyConnection(const std::string& proxyConnection);
    std::string acceptEncoding() const;
    void acceptEncoding(const std::string& acceptEncoding);
    std::string transferEncoding() const;
    void transferEncoding(const std::string& transferEncoding);
    std::string contentEncoding() const;
    void contentEncoding(const std::string& contentEncoding);

    std::string getField(const std::string& field) const;
    std::string getField(HttpField field) const;
    void setField(const std::string& field, const std::string& value);
    void setField(HttpField field, const std::string& value);

private:
    HttpMethod method_;
    Url url_;
    std::uint16_t redirects_;
    std::uint16_t timeout_;
    std::string destination_;
    HttpFields fields_;
};

}
}
}
