#pragma once

namespace c4 {
namespace net {
namespace http {

class HttpResponse
{
public:
    HttpResponse();
    HttpResponse(const HttpResponse&) = default;

    HttpResponse& operator=(const HttpResponse&) = default;

    virtual ~HttpResponse() noexcept;
};

}
}
}
