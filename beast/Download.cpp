#include <atomic>

#include <Download.hpp>

namespace c4 {
namespace net {
namespace http {

Download::Download(boost::asio::io_context& io_context) :
    io_context_(io_context),
    connectTimeout_(0),
    downloadTimeout_(0),
    lastBytesTransferred_(0)
{
}

Download::Download(boost::asio::io_context& io_context, handle_response_func_type handleResponseFunc) :
    io_context_(io_context),
    handleResponseFunc_(std::make_shared<handle_response_func_type>(handleResponseFunc)),
    connectTimeout_(0),
    downloadTimeout_(0),
    lastBytesTransferred_(0)
{
}

Download::~Download() noexcept
{
    cancel();
}

void Download::cancel()
{
    clearHandleResponseFunc();
    close();
}

void Download::close()
{
}

boost::system::error_code Download::asyncDownload(const Url& url, const std::string& path,
        size_t connectTimeout, size_t downloadTimeout)
{
    return boost::system::error_code();
}

Download::handle_response_func_type Download::handleResponseFunc() const
{
    auto func = std::atomic_load_explicit(&handleResponseFunc_, std::memory_order_acquire);
    if (func)
        return *func;
    else
        return handle_response_func_type();
}

void Download::handleResponseFunc(handle_response_func_type handleResponseFunc)
{
    std::atomic_store_explicit(&handleResponseFunc_, std::make_shared<handle_response_func_type>(handleResponseFunc),
        std::memory_order_release);
}

void Download::clearHandleResponseFunc()
{
    std::atomic_store_explicit(&handleResponseFunc_, std::shared_ptr<handle_response_func_type>(),
        std::memory_order_release);
}

}
}
}
