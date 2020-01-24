#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>

#include <boost/system/error_code.hpp>

namespace c4 {
namespace net {
namespace http {

const char* const kHTTP = "http";
const char* const kHTTPS = "https";

class Url
{
public:
    using Queries = std::unordered_map<std::string, std::string>;

    Url() :
        port_(0),
        isValid_(false)
    {};

    Url(const Url&) = default;
    Url(Url&&) = default;
    virtual ~Url() {};

    Url& operator=(const Url&) = default;
    Url& operator=(Url&&) = default;

    void scheme(const std::string& scheme) { scheme_ = scheme; };
    constexpr const std::string& scheme() const { return scheme_; };

    void userInfo(const std::string& userInfo);
    constexpr const std::string& userInfo() const { return userInfo_; };

    void domain(const std::string& domain);
    constexpr const std::string& domain() const { return domain_; };

    void port(std::uint16_t port) { port_ = port; };
    constexpr std::uint16_t port() const noexcept { return port_; };

    void path(const std::string& path) { path_ = path; };
    const std::string& path() const { return path_; };

    void queryString(const std::string& queryStr);
    void addQuery(const std::string& name, const std::string& value = std::string());
    const std::string queryString() const;

    void fragment(const std::string& fragment) { fragment_ = fragment; };
    constexpr const std::string& fragment() const { return fragment_; };

    std::string toString() const;

    bool isHttp() const;
    bool isHttps() const;

    constexpr bool isValid() const { return isValid_; };

private:
    std::string scheme_;
    std::string userInfo_;
    std::string domain_;
    std::uint16_t port_;
    std::string path_;
    Queries queries_;
    std::string fragment_;
    bool isValid_;

public:
    static Url createUrl(const std::string& urlString, boost::system::error_code& ec);
};

}
}
}
