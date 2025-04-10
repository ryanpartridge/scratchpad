#include <iostream>
#include <string>
#include <fmt/format.h>

int main()
{
    std::cout << "Hello fmt_test" << std::endl;
    //auto const shortname = std::string{"director"};
    //auto const longname = std::string{"launchdarkly-microservice"};
    constexpr char shortname[] = "director";
    constexpr char longname[] = "launchdarkly-microservice";

    std::cout << fmt::format(FMT_STRING("unmodified short name: {}"), shortname) << std::endl;
    std::cout << fmt::format(FMT_STRING("unmodified long name: {}"), longname) << std::endl;
    std::cout << fmt::format(FMT_STRING("limited short name: {:.15s}"), shortname) << std::endl;
    std::cout << fmt::format(FMT_STRING("limited long name: {:.15s}"), longname) << std::endl;
}

