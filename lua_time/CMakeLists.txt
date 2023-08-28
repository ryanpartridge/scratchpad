cmake_minimum_required(VERSION 3.22.1)
project(lua_time)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LUAJIT REQUIRED IMPORTED_TARGET luajit>=2.1.0)

set(BOOST_ROOT "/home/rpartridge/boost/1.81.0")
set(Boost_USE_STATIC_LIBS "OFF")
set(Boost_NO_SYSTEM_PATHS "ON")
set(BOOST_COMPONENTS
    filesystem
    system
    thread
)
find_package(Boost "1.81.0" REQUIRED COMPONENTS ${BOOST_COMPONENTS})

set(SOURCE_FILES
    lua_time.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC PkgConfig::LUAJIT
    PUBLIC Boost::filesystem
)
