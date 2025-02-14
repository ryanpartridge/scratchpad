cmake_minimum_required(VERSION 3.25)
project(json_exception CXX)

find_package(jsoncpp REQUIRED)

add_executable(${PROJECT_NAME} src/json_exception.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE JsonCpp::JsonCpp
)

