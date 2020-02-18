#pragma once

#include <types.hpp>

namespace c4 {
namespace net {
namespace http {

class HttpResponse
{
public:
    HttpResponse();
    explicit HttpResponse(HttpStatus status);
    HttpResponse(const HttpResponse&) = default;

    HttpResponse& operator=(const HttpResponse&) = default;

    virtual ~HttpResponse() noexcept;

    HttpStatus status() const noexcept { return status_; };
    void status(HttpStatus status);

    const HttpFields& fields() const { return fields_; };
    void fields(const HttpFields& fields);

    const std::string& payload() const { return payload_; };
    void payload(const std::string& payload);

    const std::string& destination() const { return destination_; };
    void destination(const std::string& destination);

private:
    HttpStatus status_;
    HttpFields fields_;
    std::string payload_;
    std::string destination_;
};

}
}
}
