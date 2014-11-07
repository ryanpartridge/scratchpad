/*
 * shared_ptr_swap.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>

using namespace std;

class A
{
public:
    A()
    {
        id_ = count_;
        value_ = id_;
        cout << "A default constructor [id_: " << id_ << "][value_: " << value_  << "]" << endl;
        ++count_;
    }

    A(const A& a)
    {
        id_ = count_;
        value_ = a.value_;
        cout << "A copy constructor [id_: " << id_ << "][value_: " << value_ << "]" << endl;
        ++count_;
    }

    virtual ~A()
    {
        cout << "A destructor [id_: " << id_ << "][value_: " << value_ << "]" << endl;
    }

    A& operator=(const A& a)
    {
        id_ = count_;
        value_ = a.value_;
        cout << "A assignemnt operator [id_: " << id_ << "][value_: " << value_ << "]" << endl;
        ++count_;
        return *this;
    }

    int value_;

private:
    int id_;
    static int count_;
};

int A::count_ = 0;

int main(int argc, char* argv[])
{
    cout << "starting up" << endl;
    cout << endl;

    boost::shared_ptr<A> a1 = boost::make_shared<A>();
    a1 = boost::make_shared<A>();

    cout << "make a copy of a1 via copy constructor" << endl;
    boost::shared_ptr<A> a2(a1);
    cout << endl;
    a2.reset();

    cout << "make a copy of a1 via assignment" << endl;
    boost::shared_ptr<A> a3 = a1;
    cout << endl;

    boost::weak_ptr<A> w1 = a1;
    cout << "weak pointer expired? " << (w1.expired() ? "true" : "false") << endl;
    a3 = w1.lock();
    a1.reset();
    cout << "weak pointer expired? " << (w1.expired() ? "true" : "false") << endl;

    w1.reset();
    cout << "weak pointer expired? " << (w1.expired() ? "true" : "false") << endl;
    w1 = a3;

    a3.reset();
    cout << "weak pointer expired? " << (w1.expired() ? "true" : "false") << endl;
    a3 = w1.lock();
    cout << "able to lock? " << (a3 ? "true" : "false") << endl;

    boost::optional<bool> primary;
    if (primary)
    {
        cout << "primary is set" << endl;
    }
    else
    {
        cout << "primary is not set" << endl;
    }
    primary = false;
    if (primary)
    {
        cout << "primary is set: " << (*primary ? "true" : "false") << endl;
    }
    else
    {
        cout << "primary is not set" << endl;
    }

    primary = boost::none;
    if (primary != boost::none)
    {
        cout << "primary is set: " << (*primary ? "true" : "false") << endl;
    }
    else
    {
        cout << "primary is not set" << endl;
    }

    primary = true;
    if (primary)
    {
        cout << "primary is set: " << (*primary ? "true" : "false") << endl;
    }
    else
    {
        cout << "primary is not set" << endl;
    }

    primary = boost::none;
    if (primary != boost::none)
    {
        cout << "primary is set: " << (*primary ? "true" : "false") << endl;
    }
    else
    {
        cout << "primary is not set" << endl;
    }
    cout << "all done" << endl;

    return 0;
}
