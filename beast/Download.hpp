#pragma once

#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>

#include <Url.hpp>

namespace c4 {
namespace net {
namespace http {

class Download :
    public std::enable_shared_from_this<Download>,
    boost::noncopyable
{
public:
    using handle_response_func_type = boost::function<void(const std::string&, size_t, size_t, size_t, const boost::system::error_code&)>;
    //TODO: how to do form args???

    explicit Download(boost::asio::io_context& io_context);
    Download(boost::asio::io_context& io_context, handle_response_func_type handleResponseFunc);

    virtual ~Download() noexcept;

    void cancel();
    void close();

    boost::system::error_code asyncDownload(const Url& url, const std::string& path, size_t connectTimeout = 120, size_t downloadTimeout = 300);

    handle_response_func_type handleResponseFunc() const;
    void handleResponseFunc(handle_response_func_type handleResponseFunc);
    void clearHandleResponseFunc();

private:
    using timer_type = boost::asio::steady_timer;

    boost::asio::io_context& io_context_;
    std::shared_ptr<handle_response_func_type> handleResponseFunc_;
    Url url_;
    size_t connectTimeout_;
    size_t downloadTimeout_;
    size_t lastBytesTransferred_;
};

}
}
}
