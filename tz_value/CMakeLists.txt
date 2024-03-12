cmake_minimum_required(VERSION 3.22.1)
project(tz_value)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES
    tz_value.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

