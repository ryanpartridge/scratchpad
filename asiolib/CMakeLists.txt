cmake_minimum_required(VERSION 3.5.1)
project(asiolib)

set(CMAKE_CXX_STANDARD 11)

set(BOOST_COMPONENTS
    system
    thread
)

set(BOOST_ROOT "/home/rpartridge/boost/1.72.0")
set(Boost_USE_STATIC_LIBS "OFF")
set(Boost_NO_SYSTEM_PATHS "ON")

find_package(Boost "1.72" REQUIRED COMPONENTS ${BOOST_COMPONENTS})
find_package(OpenSSL "1.0.2" REQUIRED)

set(SOURCE_FILES
    asiolib.cpp
)

set(HEADER_FILES
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(asio SHARED
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(asio
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${Boost_INCLUDE_DIRS}
    PRIVATE ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(asio
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${OPENSSL_LIBRARIES}
    pthread
)

target_compile_definitions(asio
    PRIVATE BOOST_ASIO_SEPARATE_COMPILATION
)
