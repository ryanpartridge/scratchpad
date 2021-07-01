cmake_minimum_required(VERSION 3.14)
project(template_test)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    BaseClass.cpp
    Concrete.cpp
    template_test.cpp
)

set(HEADER_FILES
    BaseClass.hpp
    Concrete.hpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
