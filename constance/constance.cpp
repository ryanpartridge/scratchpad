#include <iostream>
#include <string>

class StringHolder
{
public:
    explicit StringHolder(std::string const& value) :
        value_{value}
    {}

    StringHolder() = delete;

    auto value(std::string const& newValue) -> void
    {
        value_ = newValue;
    }

    auto value() const -> std::string const&
    {
        return value_;
    }

private:
    std::string value_;
};

class Foo
{
public:
    Foo()
    {
        std::cout << "default constructor" << std::endl;
    }

    Foo(int a, bool b, std::string const& c) :
        a_{a},
        b_{b},
        c_{c}
    {
        std::cout << "parameterized constructor" << std::endl;
    }

    virtual ~Foo()
    {
        std::cout << "destructor" << std::endl;
    }

    auto print() -> void
    {
        std::cout << "a: " << a_ << std::endl;
        std::cout << "b: " << std::boolalpha << b_ << std::endl;
        std::cout << "c: " << c_ << std::endl;
    }

private:
    int a_{0};
    bool b_{false};
    std::string c_;
};

int main(int argc, char* argv[])
{
    std::cout << "Hello, Constance!" << std::endl;

    StringHolder s1{"original"};
    std::cout << "original value: " << s1.value() << std::endl;

    std::string const& value = s1.value();
    std::string & newValue = const_cast<std::string &>(value);
    newValue = "a new value";

    std::cout << "current value: " << s1.value() << std::endl;

    const StringHolder s2{"const original"};
    std::cout << "original value: " << s2.value() << std::endl;

    std::string const& value2 = s2.value();
    std::string & newValue2 = const_cast<std::string &>(value2);
    newValue2 = "const new value";

    std::cout << "current value: " << s2.value() << std::endl;

    Foo f1;
    f1.print();
    f1 = {1, true, "hello"};
    f1.print();
    f1 = {};
    f1.print();

    return 0;
}
