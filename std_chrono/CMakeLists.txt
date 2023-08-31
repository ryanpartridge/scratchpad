cmake_minimum_required(VERSION 3.22.1)
project(std_chrono)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES
    std_chrono.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

