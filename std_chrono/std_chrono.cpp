#include <iostream>
#include <chrono>
#include <ctime>

int main(int argc, char* argv[])
{
    std::cout << "entering main" << std::endl;

    auto sourceTimePoint = std::chrono::system_clock::now() + std::chrono::minutes(5);
    auto sourceDuration = std::chrono::duration_cast<std::chrono::seconds>(sourceTimePoint.time_since_epoch());
    auto sourceTimeT = std::chrono::system_clock::to_time_t(sourceTimePoint);

    std::cout << "Source time (string): " << std::ctime(&sourceTimeT);
    std::cout << "Normalized source time: " << sourceDuration.count() << std::endl;

    auto targetDuration = std::chrono::seconds(1693516746);
    auto targetTimePoint = std::chrono::system_clock::time_point(targetDuration);
    auto targetTimeT = std::chrono::system_clock::to_time_t(targetTimePoint);
    std::cout << "Target time (string): " << std::ctime(&targetTimeT);
    std::cout << "Target time has " << (targetTimePoint < std::chrono::system_clock::now() ? "" : "NOT ") << "expired" << std::endl;

    std::chrono::steady_clock::time_point const defaultTP;
    std::chrono::steady_clock::time_point const minTP{std::chrono::steady_clock::time_point::min()};
    std::chrono::steady_clock::time_point const maxTP{std::chrono::steady_clock::time_point::max()};

    if (defaultTP == minTP)
    {
        std::cout << "default is the same as min" << std::endl;
    }
    else if (defaultTP > minTP)
    {
        std::cout << "default is greater than min" << std::endl;
    }
    else
    {
        std::cout << "default is less than min" << std::endl;
    }

    if (defaultTP == maxTP)
    {
        std::cout << "default is the same as max" << std::endl;
    }
    else if (defaultTP > maxTP)
    {
        std::cout << "default is greater than max" << std::endl;
    }
    else
    {
        std::cout << "default is less than max" << std::endl;
    }

    auto offsetTP = defaultTP + std::chrono::hours(48);
    std::cout << "offset is greater than default: " << std::boolalpha << (offsetTP > defaultTP) << std::endl;
    offsetTP = {};
    std::cout << "offset is the same as default: " << std::boolalpha << (offsetTP == defaultTP) << std::endl;

    std::cout << "exiting main" << std::endl;
    return 0;
}
