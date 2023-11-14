cmake_minimum_required(VERSION 3.21.1)
project(ssl_verify)

set(CMAKE_CXX_STANDARD 11)

set(BOOST_COMPONENTS
    system
    thread
)

set(BOOST_ROOT "/home/rpartridge/boost/1.81.0")
set(Boost_USE_STATIC_LIBS "OFF")
set(Boost_NO_SYSTEM_PATHS "ON")

find_package(Boost "1.81.0" REQUIRED COMPONENTS ${BOOST_COMPONENTS})
find_package(OpenSSL "3.0" REQUIRED)

set(SOURCE_FILES
    ssl_verify.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${Boost_INCLUDE_DIRS}
    PRIVATE ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${OPENSSL_LIBRARIES}
    pthread
)

