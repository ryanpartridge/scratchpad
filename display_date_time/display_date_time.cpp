#include <ctime>
#include <iostream>
#include <ostream>
#include <time.h>

int main(int argc, char* argv[])
{
    std::cout << "Current date and time" << std::endl;
    timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
    {
        std::cerr << "Error getting the date/time" << std::endl;
        return 1;
    }

    tm* currentTime = localtime(&ts.tv_sec);
    if (currentTime == nullptr)
    {
        std::cerr << "Error converting to localtime" << std::endl;
        return 1;
    }

    std::cout << "Current year: " << (1900 + currentTime->tm_year) << std::endl;
    std::cout << "Logging timestamp: " << (1900 + currentTime->tm_year) << "-" << (currentTime->tm_mon + 1) << "-" << currentTime->tm_mday << " " <<
        currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << std::endl;
    return 0;
}
