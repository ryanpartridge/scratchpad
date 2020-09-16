cmake_minimum_required(VERSION 3.5.1)
project(index_replace LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)

set(BOOST_COMPONENTS
    system
)
set(BOOST_ROOT "$ENV{HOME}/boost/1.72.0")
set(Boost_USE_STATIC_LIBS "OFF")
set(Boost_NO_SYSTEM_PATHS "ON")
set(BOOST_VERSION_MAJOR 1)
set(BOOST_VERSION_MINOR 72)
#set(Boost_COMPILER "-gcc")

find_package(Boost ${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR} EXACT REQUIRED
    COMPONENTS
    ${BOOST_COMPONENTS}
)

set(SOURCE_FILES
    index_replace.cpp
)

set(HEADER_FILES
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_SYSTEM_LIBRARY}
)
