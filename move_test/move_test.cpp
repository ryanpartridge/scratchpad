#include <iostream>
#include <tuple>
#include <string>

using my_tuple = std::tuple<uint32_t, std::string>;

void receiveByMove(my_tuple&& value)
{
    std::cout << "int: " << std::get<0>(value) << std::endl << "string: " << std::get<1>(value) << std::endl << std::endl;
}

my_tuple returnByMove(uint32_t intVal, std::string strVal)
{
    // let the compiler employ return value optimization
    my_tuple t(intVal, strVal);
    return t;
}

int main(int argc, char* argv[])
{
    std::cout << "starting move_test" << std::endl;

    my_tuple t(0, std::string("hello world"));
    receiveByMove(std::move(t));
    t = returnByMove(1, std::string("hello again"));

    return 0;
}
