#include <iostream>
#include <tuple>
#include <string>
#include <deque>

using my_tuple = std::tuple<uint32_t, std::string>;

class MyType
{
public:
    MyType() = default;
    explicit MyType(std::string const& value) :
        value_{value}
    {
        std::cout << "default constructor (" << value_ << ")" << std::endl;
    }

    MyType(MyType const& other) :
        value_{other.value_}
    {
        std::cout << "const copy constructor (" << value_ << ")" << std::endl;
    }

    MyType(MyType& other) :
        value_{other.value_}
    {
        std::cout << "non const copy constructor (" << value_ << ")" << std::endl;
    }

    MyType(MyType&& other)
    {
        value_ = std::move(other.value_);
        std::cout << "move constructor (" << value_ << ")" << std::endl;
    }

    virtual ~MyType()
    {
        std::cout << "destructor (" << value_ << ")" << std::endl;
    }

    std::string const& value() const
    {
        return value_;
    }

    void print()
    {
        std::cout << "value: " << value_ << std::endl;
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

    std::deque<MyType> mq;
    mq.emplace_back("m1");
    mq.emplace_back("m2");
    mq.emplace_back("m3");

    MyType m1 = std::move(mq.front());
    mq.pop_front();
    m1.print();

    mq.clear();
    mq.push_back(std::move(m1));

    return 0;
}
