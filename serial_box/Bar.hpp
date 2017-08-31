#ifndef BOO_HPP
#define BOO_HPP

#include <string>

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include "Base.hpp"

class Bar : public Base
{
public:
    Bar() :
        text_("Hello world!"),
        type_('a')
    {}
    virtual ~Bar() {}

    virtual std::string description()
    {
        std::ostringstream out;
        out << "Bar [text: " << text_ << "][type: " << type_ << "]";
        return out.str();
    }

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned /* version */)
    {
        // serialize the base class
        ar & boost::serialization::base_object<Base>(*this);
        ar & text_;
        ar & type_;
    }

    std::string text_;
    char type_;
};

#endif
