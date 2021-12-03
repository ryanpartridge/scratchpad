cmake_minimum_required(VERSION 3.16.3)
project(child_process)

set(CMAKE_CXX_STANDARD 14)

set(BOOST_ROOT "/home/rpartridge/boost/1.72.0")
set(Boost_USE_STATIC_LIBS "OFF")
set(Boost_NO_SYSTEM_PATHS "ON")

find_package(Boost "1.72.0" REQUIRED)

set(SOURCE_FILES
    child_process.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${Boost_THREAD_LIBRARY}
    PRIVATE ${Boost_SYSTEM_LIBRARY}
    PRIVATE pthread
)