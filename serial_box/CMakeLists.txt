cmake_minimum_required(VERSION 3.0)
project(serial_box LANGUAGES C CXX)

#set(Boost_NO_SYSTEM_PATHS "ON")
set(BOOST_VERSION_MAJOR 1)
set(BOOST_VERSION_MINOR 62)
#set(Boost_COMPILER "-gcc")
set(BOOST_ROOT "$ENV{HOME}/boost/1.62.0")
set(BOOST_COMPONENTS
    chrono
    context
    coroutine
    date_time
    filesystem
    regex
    serialization
    system
    thread
)

find_package(Boost ${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR} EXACT REQUIRED
    COMPONENTS
    ${BOOST_COMPONENTS}
)

set(SOURCE_FILES
    serial_box.cpp
)

set(HEADER_FILES
    Base.hpp
    Foo.hpp
    Bar.hpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_SERIALIZATION_LIBRARY}
)
