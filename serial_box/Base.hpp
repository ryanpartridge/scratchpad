#ifndef BASE_HPP
#define BASE_HPP

#include <string>

#include <boost/serialization/access.hpp>

class Base
{
public:
    Base() {}
    virtual ~Base() {}

    virtual std::string description() = 0; 

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& /* ar */, const unsigned /* version */)
    {
    }
};

#endif
