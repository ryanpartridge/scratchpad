#include <iostream>
#include <cstdlib>
#include <functional>

#include "DumbExitPlugin.hpp"

extern "C" __attribute__ ((visibility ("default"))) CreatePluginCallback GetCreationFunction(void) { return &DumbExitPlugin::Create; }

void onUnloadFree()
{
    std::cout << "DumbExitPlugin unloaded (free function)" << std::endl;
}

IExitPlugin* DumbExitPlugin::Create()
{
    return new DumbExitPlugin();
}

DumbExitPlugin::DumbExitPlugin() :
    IExitPlugin()
{
    std::cout << "DumbExitPlugin constructor" << std::endl;
    // UnloadCallback callback = onUnloadFree;
    // atexit(onUnloadFree);
    // auto exitFunction = std::function<void()>(std::bind(&DumbExitPlugin::onUnload, this));
    // UnloadCallback callback = *exitFunction.target<void(*)()>();
    // atexit(*exitFunction.target<void(*)()>());
    // UnloadCallback callback = &DumbExitPlugin::onUnloadStatic;
    UnloadCallback callback = [](){ std::cout << "DumbExitPlugin unloaded (lambda function)" << std::endl; };
    atexit(callback);
}

DumbExitPlugin::~DumbExitPlugin()
{
    std::cout << "DumbExitPlugin destructor" << std::endl;
}

void DumbExitPlugin::doSomething()
{
    std::cout << "DumbExitPlugin doing something" << std::endl;
}

void DumbExitPlugin::onUnload()
{
    std::cout << "DumbExitPlugin unloaded (member function)" << std::endl;
}

void DumbExitPlugin::onUnloadStatic()
{
    std::cout << "DumbExitPlugin unloaded (static function)" << std::endl;
}
