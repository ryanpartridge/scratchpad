#include <iostream>
#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

class ForcedResolver :
    public std::enable_shared_from_this<ForcedResolver>,
    boost::noncopyable
{
public:
    ForcedResolver() = delete;

    explicit ForcedResolver(boost::asio::io_context& io_context) :
        io_context_(io_context),
        resolver_(io_context)
    {}

    virtual ~ForcedResolver() = default;

    void doAsyncResolve(const std::string& domain, boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4())
    {
        auto service = std::string("http");

        auto instance = shared_from_this();
        resolver_.async_resolve(
            protocol,
            domain,
            service,
            [instance](const boost::system::error_code& ec, const boost::asio::ip::tcp::resolver::results_type& endpoints)
            {
                if (ec)
                {
                    std::cerr << "Error in the resolve operation: " << ec.message() << std::endl;
                    return;
                }

                for (const auto& endpoint : endpoints)
                {
                    std::cout << endpoint.endpoint() << std::endl;
                }
                std::cout << "finished the resolve operation" << std::endl;
            }
        );
    }

private:
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::resolver resolver_;
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "ERROR: must specify a domain" << std::endl;
        return 1;
    }

    if (argc > 3)
    {
        std::cerr << "ERROR: too many parameters specified" << std::endl;
        return 1;
    }

    std::cout << "Running ipv4_protocol" << std::endl;

    boost::asio::io_context io_context;
    
    std::cout << "running the io_context" << std::endl;
    std::shared_ptr<ForcedResolver> resolver = std::make_shared<ForcedResolver>(io_context);

    std::unique_ptr<boost::asio::ip::tcp> protocol;

    if (argc == 3)
    {
        auto protocolString = std::string(argv[2]);
        if (protocolString == "6")
        {
            protocol = std::unique_ptr<boost::asio::ip::tcp>(new boost::asio::ip::tcp(boost::asio::ip::tcp::v6()));
        }
    }

    if (!protocol)
    {
        protocol = std::unique_ptr<boost::asio::ip::tcp>(new boost::asio::ip::tcp(boost::asio::ip::tcp::v4()));
    }
    resolver->doAsyncResolve(argv[1], *protocol);
    io_context.run();
    std::cout << "io_context finished all work" << std::endl;

    return 0;
}
