#include <iostream>
#include <lua.hpp>
#include <boost/filesystem.hpp>

int main(int argc, char* argv[])
{
    std::cout << "It's Lua Time!" << std::endl;

    if (argc < 2)
    {
        std::cerr << "No Lua script file specified" << std::endl;
        exit(1);
    }
    std::string luaFilePath = argv[1];
    
    boost::system::error_code ec;
    auto exists = boost::filesystem::exists(luaFilePath, ec);
    if (!exists || ec)
    {
        std::cerr << "Lua script file " << luaFilePath << " does not exist or cannot be accessed" << std::endl;
        exit(1);
    }

    lua_State* state = luaL_newstate();
    if (state == nullptr)
    {
        std::cerr << "Error creating the Lua stack" << std::endl;
        exit(1);
    }
    luaL_openlibs(state);

    auto result = luaL_dofile(state, luaFilePath.c_str());
    if (result != 0)
    {
        std::cerr << "Error loading contents of " << luaFilePath << std::endl;
        exit(1);
    }

    lua_getglobal(state, "testEmbeddedLua");
    result = lua_pcall(state, 0, LUA_MULTRET, 0);
    if (result != 0)
    {
        if (lua_isstring(state, -1))
        {
            auto errorString = lua_tostring(state, -1);
            std::cerr << "Error calling function: " << errorString << std::endl;
        }
    }
    else
    {
        if (lua_isnumber(state, -1))
        {
            auto returnValue = lua_tonumber(state, -1);
            std::cout << "Function return value: " << returnValue << std::endl;
        }
    }
    lua_pop(state, 1);

    lua_close(state);
}
