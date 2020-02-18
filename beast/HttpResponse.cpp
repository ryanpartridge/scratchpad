#include <HttpResponse.hpp>

namespace c4 {
namespace net {
namespace http {

HttpResponse::HttpResponse() :
    status_(toStatus(200))
{
}

HttpResponse::HttpResponse(HttpStatus status) :
    status_(status)
{
}

HttpResponse::~HttpResponse() noexcept
{
}

void HttpResponse::status(HttpStatus status)
{
    status_ = status;
}

void HttpResponse::destination(const std::string& destination)
{
    destination_ = destination;
}

void HttpResponse::payload(const std::string& payload)
{
    payload_ = payload;
}

}
}
}
