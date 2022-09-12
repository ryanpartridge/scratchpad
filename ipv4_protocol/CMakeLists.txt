cmake_minimum_required(VERSION 3.22.1)
project(ipv4_protocol)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BOOST_COMPONENTS
    system
    thread
)

set(BOOST_ROOT "/home/rpartridge/boost/1.75.0")
set(Boost_USE_STATIC_LIBS "OFF")
set(Boost_NO_SYSTEM_PATHS "ON")

find_package(Boost "1.75" REQUIRED COMPONENTS ${BOOST_COMPONENTS})

set(SOURCE_FILES
    ipv4_protocol.cpp
)

set(HEADER_FILES
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    pthread
)
