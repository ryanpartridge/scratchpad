#ifndef FOO_HPP
#define FOO_HPP

#include <string>
#include <sstream>

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include "Base.hpp"

class Foo : public Base
{
public:
    Foo() :
        flag_(false),
        count_(0)
    {}
    virtual ~Foo() {}

    virtual std::string description()
    {
        std::ostringstream out;
        out << "Foo [flag: " << flag_ << "][count: " << count_ << "]";
        return out.str();
    }

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned /* version */)
    {
        // serialize the base class
        ar & boost::serialization::base_object<Base>(*this);
        ar & flag_;
        ar & count_;
    }

    bool flag_;
    size_t count_;
};

#endif
