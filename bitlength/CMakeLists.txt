cmake_minimum_required(VERSION 3.5.1)
project(bitlength)

set(CMAKE_CXX_STANDARD 11)

set(BOOST_ROOT "/home/rpartridge/boost/1.72.0")
set(Boost_USE_STATIC_LIBS "OFF")
set(Boost_NO_SYSTEM_PATHS "ON")

find_package(Boost "1.72.0" REQUIRED)

set(SOURCE_FILES
    bitlength.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${Boost_INCLUDE_DIRS}
)

set(CMAKE_C_FLAGS "-m32")
set(CMAKE_CXX_FLAGS "-m32")
