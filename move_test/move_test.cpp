#include <iostream>
#include <tuple>
#include <string>

using my_tuple = std::tuple<uint32_t, std::string>;

class MyType
{
public:
    MyType() = default;
    explicit MyType(std::string const& value) :
        value_{value}
    {}

    MyType(MyType const& other) :
        value_{other.value_}
    {
        std::cout << "const copy constructor" << std::endl;
    }

    MyType(MyType& other) :
        value_{other.value_}
    {
        std::cout << "non const copy constructor" << std::endl;
    }

    MyType(MyType&& other)
    {
        std::cout << "move constructor" << std::endl;
        value_ = std::move(other.value_);
    }

    virtual ~MyType() = default;

    std::string const& value() const
    {
        return value_;
    }

private:
    std::string value_;
};

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

void passByConstRef(MyType const& m)
{
    MyType m2 = std::move(m);
}

void passByNonConstRef(MyType& m)
{
    MyType m2 = std::move(m);
}

int main(int argc, char* argv[])
{
    std::cout << "starting move_test" << std::endl;

/*
    my_tuple t(0, std::string("hello world"));
    receiveByMove(std::move(t));
    t = returnByMove(1, std::string("hello again"));
*/
    MyType m1("hello");
    std::cout << "value of m1: " << m1.value() << std::endl;
    passByConstRef(m1);
    std::cout << "value of m1 after const ref function: " << m1.value() << std::endl;
    passByNonConstRef(m1);
    std::cout << "value of m1 after non const ref function: " << m1.value() << std::endl;

    return 0;
}
