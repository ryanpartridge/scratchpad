#pragma once

#include <string>
#include <cstdint>
#include <cctype>
#include <algorithm>

#include <boost/beast/http/status.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/beast/http/field.hpp>
#include <boost/beast/http/fields.hpp>

namespace c4 {
namespace net {
namespace http {

    using HttpStatus = boost::beast::http::status;
    using HttpMethod = boost::beast::http::verb;
    using HttpField = boost::beast::http::field;
    using HttpFields = boost::beast::http::fields;

    inline HttpStatus toStatus(uint16_t value)
    {
        return boost::beast::http::int_to_status(value);
    }

    inline HttpMethod toMethod(const std::string& value)
    {
        std::string upper;
        upper.resize(value.size());
        std::transform(value.begin(), value.end(), upper.begin(), ::toupper);
        return boost::beast::http::string_to_verb(upper);
    }

    // the string must be an exact match, or the lookup will fail
    inline HttpField toField(const std::string& value)
    {
        return boost::beast::http::string_to_field(value);
    }

    inline std::string toString(HttpStatus status)
    {
        return boost::beast::http::obsolete_reason(status).to_string();
    }

    inline std::string toString(HttpMethod method)
    {
        return boost::beast::http::to_string(method).to_string();
    }

    inline std::string toString(HttpField field)
    {
        return boost::beast::http::to_string(field).to_string();
    }

}
}
}
