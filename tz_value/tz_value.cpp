#include <iostream>
#include <ctime>

int main(int argc, char* argv[])
{
    std::cout << "starting tz_value" << std::endl;

    tzset();

    return 0;
}
