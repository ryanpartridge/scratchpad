cmake_minimum_required(VERSION 3.5.1)
project(pipes)

set(BOOST_VERSION_MAJOR 1)
set(BOOST_VERSION_MINOR 67)
set(BOOST_VERSION_SUBMINOR 0)
set(BOOST_ROOT $ENV{HOME}/boost/1.67.0)
set(BOOST_COMPONENTS
    system
    thread
)
find_package(Boost ${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR}.${BOOST_VERSION_PATCH} EXACT REQUIRED
    COMPONENTS
    ${BOOST_COMPONENTS}
)

set(SOURCE_FILES
    pipes.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)
