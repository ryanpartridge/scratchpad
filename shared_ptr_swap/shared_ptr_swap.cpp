/*
 * shared_ptr_swap.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/shared_ptr.hpp>
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

    cout << "all done" << endl;
    return 0;
}
