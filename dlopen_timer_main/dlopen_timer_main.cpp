/*
 * dlopen_timer_main.cpp
 *
 *  Created on: Oct 10, 2013
 *      Author: rpartridge
 */

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/bind.hpp>
#include <dlfcn.h>

using namespace std;

void onTimer(const boost::system::error_code& ec)
{
    cout << "[onTimer] timer expired" << endl;
}

typedef void (*lib_function)(boost::asio::io_service&);
typedef void (*void_function)(void);

int main(int argc, char* argv[])
{
    cout << "[main] enter main" << endl;
    boost::asio::io_service io;

    cout << "[main] setting timer" << endl;
    boost::asio::steady_timer timer(io);
    timer.expires_from_now(boost::chrono::seconds(5));
    timer.async_wait(boost::bind(&onTimer, boost::asio::placeholders::error));
    cout << "[main] address of io_service: " << &io << endl;

    // load the library
    cout << "[main] opening the library" << endl;
    dlerror();
    void* handle = dlopen("/home/rpartridge/github/scratchpad/dlopen_timer_lib/Debug/libdlopen_timer_lib.so", RTLD_LAZY);
    if (!handle)
    {
        cout << "[main] error opening the library: " << dlerror() << endl;
    }
    else
    {
        // get the function
        dlerror();
        lib_function func = (lib_function)dlsym(handle, "libStartTimer");
        const char* error;
        if ((error = dlerror()))
        {
            cout << "[main] error getting function handle: " << error << endl;
        }
        else
        {
            // call the function
            func(io);
        }
    }

    cout << "[main] call io.run()" << endl;
    io.run();

    if (handle)
    {
        cout << "[main] closing the library" << endl;
        dlclose(handle);
    }
    cout << "[main] exit main" << endl;
    return 0;
}
