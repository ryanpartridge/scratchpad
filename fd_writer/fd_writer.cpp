#include <unistd.h>
#include <errno.h>

#include <cstring>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <sstream>
#include <ctime>

int getIntArg(int argc, char* argv[], int argIndex, int defaultValue = 0)
{
    auto value(defaultValue);
    if (argc > argIndex)
    {
        errno = 0;
        char* endPtr = nullptr;
        auto converted = strtol(argv[argIndex], &endPtr, 10);
        if (errno != 0 || endPtr == argv[argIndex])
        {
            std::cerr << "Invalid value specified for argument " << argIndex << std::endl;
            return -1;
        }
        else
        {
            value = static_cast<int>(converted);
        }
    }
    return value;
}

int main(int argc, char* argv[])
{
    auto fdNum = getIntArg(argc, argv, 1, 1);
    auto sleepValue = getIntArg(argc, argv, 2, 250);

    std::cout << "Using FD: " << fdNum << " and sleep value: " << sleepValue << std::endl;

    for (auto i = 0; i < 50; ++i)
    {
        auto messageStream = std::ostringstream();
        messageStream << "Hello world! (" << i << ")" << std::endl;
        auto message = messageStream.str();
        auto res = write(fdNum, message.c_str(), message.size());
        if (res == -1)
        {
            std::cerr << "Error writing to FD " << fdNum << " (" << errno << "): " << strerror(errno) << std::endl;
            break;
        }

        auto sleepSeconds(0);
        auto sleepMillis = sleepValue;
        if (sleepValue > 999)
        {
            sleepSeconds = sleepValue / 1000;
            sleepMillis = sleepValue % 1000;
        }
        auto sleepTime = timespec{sleepSeconds, sleepMillis * 1000000};
        nanosleep(&sleepTime, nullptr);
    }

    std::cerr << "This is the error output for the fd_writer process." << std::endl;
    return 0;
}
