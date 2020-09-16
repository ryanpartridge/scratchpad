cmake_minimum_required(VERSION 3.14)
project(fd_writer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    fd_writer.cpp
)

set(HEADER_FILES
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

