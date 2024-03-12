#include <iostream>
#include <ctime>
#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>

bool is_dst()
{
    auto now = time(nullptr);
    auto ptm = localtime(&now);
    if (ptm != nullptr)
    {
        return ptm->tm_isdst > 0;
    }
    return false;
}

std::string timezoneName()
{
    char timezoneBuffer[16];
    memset(timezoneBuffer, 0, sizeof(timezoneBuffer));
    tzset();
    std::string value;
    if (is_dst())
    {
        value = std::string(tzname[1], strnlen(tzname[1], sizeof(timezoneBuffer) - 1));
    }
    else
    {
        value = std::string(tzname[1], strnlen(tzname[0], sizeof(timezoneBuffer) - 1));
    }
    return value;
}

std::string timezoneNameNew()
{
    auto now = time(nullptr);
    auto ptm = localtime(&now);
    std::ostringstream tzStream;
    tzStream << std::put_time(ptm, "%Z");
    return tzStream.str();
}

int main(int argc, char* argv[])
{
    std::cout << "starting tz_value" << std::endl;
    std::cout << "timezone: " << timezoneName() << std::endl;
    std::cout << "timezone (new): " << timezoneNameNew();

    return 0;
}
