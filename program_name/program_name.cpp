#include <iostream>

//extern char* program_invocation_short_name;

int main(int argc, char* argv[])
{
    char* prog_name = program_invocation_short_name;
    std::cout << "program short name: " << prog_name << std::endl;
    return 0;
}
