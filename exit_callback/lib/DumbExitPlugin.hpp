#include <IExitPlugin.hpp>

struct DumbExitPlugin :
    public virtual IExitPlugin
{
    static IExitPlugin* Create();

    DumbExitPlugin();
    virtual ~DumbExitPlugin();

    virtual void doSomething();
    void onUnload();

    static void onUnloadStatic();
};
