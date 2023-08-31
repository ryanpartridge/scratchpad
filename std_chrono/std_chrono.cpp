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

    std::cout << "exiting main" << std::endl;
    return 0;
}
