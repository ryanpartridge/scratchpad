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

using namespace std;

class A
{
public:
    A()
    {
        id_ = count_;
        cout << "A default constructor (" << id_ << ")" << endl;
        ++count_;
    }

    A(const A& a)
    {
        id_ = a.id_;
        cout << "A copy constructor (" << id_ << ")" << endl;
    }

    virtual ~A()
    {
        cout << "A destructor (" << id_ << ")" << endl;
    }

    A& operator=(const A& a)
    {
        id_ = a.id_;
        cout << "A assignemnt operator (" << id_ << ")" << endl;
        return *this;
    }

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

    cout << "all done" << endl;
    return 0;
}
