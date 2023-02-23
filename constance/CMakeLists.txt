cmake_minimum_required(VERSION 3.22.1)
project(constance)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    constance.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
