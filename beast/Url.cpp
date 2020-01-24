#include <Url.hpp>
#include <vector>
#include <sstream>
#include <iostream>

#include <boost/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

namespace c4 {
namespace net {
namespace http {

void Url::domain(const std::string& domain)
{
    auto userInfoIndex = domain.find("@");
    if (userInfoIndex != std::string::npos)
    {
        userInfo_ = domain.substr(0, userInfoIndex);
        ++userInfoIndex;
    }
    else
    {
        userInfoIndex = 0;
    }

    auto index = domain.find_last_of(":");
    if (index != std::string::npos)
    {
        domain_ = domain.substr(userInfoIndex, index - userInfoIndex);
        try
        {
            port_ = boost::lexical_cast<std::uint16_t>(domain.substr(index + 1));
        }
        catch(const boost::bad_lexical_cast&)
        {
        }
    }
    else
    {
        if (userInfoIndex > 0)
        {
            domain_ = domain.substr(userInfoIndex);
        }
        else
        {
            domain_ = domain;
        }
    }

    if (port_ == 0)
    {
        if (isHttps())
        {
            port_ = 443;
        }
        else
        {
            port_ = 80;
        }
    }
}

void Url::queryString(const std::string& queryStr)
{
    queries_.clear();
    std::vector<std::string> parts;
    boost::algorithm::split(parts, queryStr, boost::algorithm::is_any_of("&"), boost::algorithm::token_compress_on);
    for (const auto& query: parts)
    {
        auto index = query.find('=');
        if (index != std::string::npos)
        {
            queries_.emplace(query.substr(0, index), query.substr(index + 1));
        }
        else
        {
            queries_.emplace(query, std::string());
        }
    }
}

void Url::addQuery(const std::string& name, const std::string& value)
{
    const auto it = queries_.find(name);
    if (it != queries_.end())
    {
        queries_.erase(it);
    }
    queries_.emplace(name, value);
}

const std::string Url::queryString() const
{
    if (queries_.empty())
    {
        return std::string();
    }

    std::ostringstream out;
    out << "?";
    for (auto it = queries_.begin(); it != queries_.end(); ++it)
    {
        if (it != queries_.begin())
        {
            out << "&";
        }
        out << it->first;
        if (!(it->second.empty()))
        {
            out << "=" << it->second;
        }
    }
    return out.str();
}

std::string Url::toString() const
{
    std::ostringstream out;
    out << scheme_ << "://";
    if (!userInfo_.empty())
    {
        out << userInfo_ << "@";
    }
    out << domain_;
    if ((isHttp() && port_ != 80) || (isHttps() && port_ != 443))
    {
        out << ":" << port_;
    }
    if (!boost::algorithm::starts_with(path_, "/"))
    {
        out << "/";
    }
    out << path_ << queryString();
    if (!fragment_.empty())
    {
        out << "#" << fragment_;
    }
    return out.str();
}

bool Url::isHttp() const
{
    return (boost::iequals(scheme_, kHTTP));
}

bool Url::isHttps() const
{
    return (boost::iequals(scheme_, kHTTPS));
}

Url Url::createUrl(const std::string& urlString, boost::system::error_code& ec)
{
    try
    {
        /*
        * regular expression from RFC3986 (https://tools.ietf.org/html/rfc3986#page-50)
        * ^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?
        */
        static const auto urlRegex = boost::regex("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?", boost::regex_constants::perl);
        boost::smatch parts;
        if (boost::regex_match(urlString, parts, urlRegex))
        {
            Url url;
            if (parts[2].length() > 0)
            {
                url.scheme(parts[2]);
            }
            if (parts[4].length() > 0)
            {
                url.domain(parts[4]);
            }
            if (parts[5].length() > 0)
            {
                url.path(parts[5]);
            }
            else
            {
                url.path("/");
            }
            if (parts[7].length() > 0)
            {
                url.queryString(parts[7]);
            }
            if (parts[9].length() > 0)
            {
                url.fragment(parts[9]);
            }
            url.isValid_ = true;
            return url;
        }
        else
        {
            ec = boost::system::errc::make_error_code(boost::system::errc::invalid_argument);
            return Url();
        }
        
    }
    catch(const boost::bad_expression&)
    {
        ec = boost::system::errc::make_error_code(boost::system::errc::invalid_argument);
        return Url();
    }
}

}
}
}
