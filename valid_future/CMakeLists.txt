cmake_minimum_required(VERSION 3.16)
project(valid_future)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    valid_future.cpp
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

target_link_libraries(${PROJECT_NAME}
    pthread
)