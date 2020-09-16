cmake_minimum_required(VERSION 3.5.1)
project(beast)

set(CMAKE_CXX_STANDARD 11)

set(BOOST_COMPONENTS
    filesystem
    regex
    system
    thread
)

set(BOOST_ROOT "/home/rpartridge/boost/1.72.0")
set(Boost_USE_STATIC_LIBS "OFF")
set(Boost_NO_SYSTEM_PATHS "ON")

find_package(Boost "1.72" REQUIRED COMPONENTS ${BOOST_COMPONENTS})
find_package(OpenSSL "1.0.2" REQUIRED)

set(SOURCE_FILES
    beast.cpp
    Download.cpp
    HttpClient.cpp
    HttpRequest.cpp
    HttpResponse.cpp
    Url.cpp
)

set(HEADER_FILES
    Download.hpp
    HttpClient.hpp
    HttpRequest.hpp
    HttpResponse.hpp
    memory.hpp
    types.hpp
    Url.hpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${Boost_INCLUDE_DIRS}
    PRIVATE ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_ATOMIC_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${OPENSSL_LIBRARIES}
    pthread
)
#    /home/rpartridge/github/scratchpad/asiolib/build/libasio.so

#target_compile_definitions(${PROJECT_NAME}
#    PRIVATE BOOST_ASIO_SEPARATE_COMPILATION
#    PRIVATE BOOST_ASIO_DYN_LINK
#)
