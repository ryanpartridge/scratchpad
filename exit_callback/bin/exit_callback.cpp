#include <iostream>
#include <memory>

#include <boost/filesystem/path.hpp>
#include <boost/dll/shared_library.hpp>

#include "IExitPlugin.hpp"

int main(int argc, char* argv[])
{
    std::cout << "starting exit_callback" << std::endl;
    auto libraryPath = boost::filesystem::path("../lib/libexitlib.so");
    auto library = boost::dll::shared_library(libraryPath);

    std::unique_ptr<IExitPlugin> plugin;
    if (library.has("GetCreationFunction"))
    {
        auto createFunction = library.get<CreatePluginCallback()>("GetCreationFunction");
        auto createPluginActual = createFunction();
        plugin = std::unique_ptr<IExitPlugin>(createPluginActual());
        plugin->doSomething();
        plugin.reset();
        library.unload();
    }
    std::cout << "exiting exit_callback" << std::endl;
}
