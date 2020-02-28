#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include <types.hpp>
#include <Url.hpp>
#include <HttpRequest.hpp>
#include <HttpResponse.hpp>
#include <HttpClient.hpp>

void handleResponse(const c4::net::http::HttpResponse& response, const boost::system::error_code& ec)
{
    if (ec)
    {
        std::cout << "Error processing the request: " << ec.message() << std::endl;
    }
    else
    {
        if (response.status() != c4::net::http::HttpStatus::ok)
        {
            std::cout << "HTTP status: " << response.status() << std::endl;
        }
        else
        {
            std::cout << "Response body: " << std::endl;
            std::cout << response.payload() << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    /*
    c4::net::http::HttpStatus status = c4::net::http::toStatus(404);
    c4::net::http::HttpMethod method = c4::net::http::toMethod("get");
    c4::net::http::HttpField field = c4::net::http::toField("Authorization");
    std::cout << "Status phrase: " << status << std::endl;
    std::cout << "Method: " << method << std::endl;
    std::cout << "Field: " << field << std::endl;
    */

    //std::string urlString = "https://user:password@www.google.com:8443/some/path/here?key1=value1&keynovalue=&key2=value2#small_fragment";
    //std::string urlString = "https://www.google.com:8443/some/path/here?key1=value1&keynovalue=&key2=value2#small_fragment";
    //std::string urlString = "https://user:password@www.google.com/some/path/here?key1=value1&keynovalue=&key2=value2#small_fragment";
    //std::string urlString = "https://www.google.com/some/path/here?key1=value1&keynovalue=&key2=value2#small_fragment";
    std::string urlString = "http://www.google.com";
    std::cout << "Input URL: " << urlString << std::endl;
    std::cout << std::endl;
    boost::system::error_code ec;
    auto url = c4::net::http::Url::createUrl(urlString, ec);
    std::cout << "scheme: " << url.scheme() << std::endl;
    std::cout << "userInfo: " << url.userInfo() << std::endl;
    std::cout << "domain: " << url.domain() << std::endl;
    std::cout << "port: " << url.port() << std::endl;
    std::cout << "path: " << url.path() << std::endl;
    std::cout << std::endl;
    std::cout << "Output URL: " << url.toString() << std::endl;

    c4::net::http::HttpRequest req(c4::net::http::HttpMethod::get, url);

    boost::asio::io_context io_context;
    auto client = std::make_shared<c4::net::http::HttpClient>(io_context, &handleResponse);
    ec = client->asyncRequest(req);
    if (ec)
    {
        std::cerr << "Error on request: " << ec.message() << std::endl;
    }

    io_context.run();

    return 0;
}
