struct IExitPlugin
{
    IExitPlugin() = default;
    virtual ~IExitPlugin() = default;
    virtual void doSomething() = 0;
};

using CreatePluginCallback = IExitPlugin* (*)();
using UnloadCallback = void (*)();
