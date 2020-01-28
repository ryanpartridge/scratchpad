cmake_minimum_required(VERSION 3.5.1)
project(beast)

set(CMAKE_CXX_STANDARD 11)

set(BOOST_COMPONENTS
    regex
    system
    thread
)

set(BOOST_ROOT "/home/rpartridge/boost/1.67.0")
set(Boost_USE_STATIC_LIBS "OFF")
set(Boost_NO_SYSTEM_PATHS "ON")

find_package(Boost "1.67" REQUIRED COMPONENTS ${BOOST_COMPONENTS})

set(SOURCE_FILES
    beast.cpp
    HttpClient.cpp
    HttpRequest.cpp
    Url.cpp
)

set(HEADER_FILES
    HttpClient.hpp
    HttpRequest.hpp
    HttpResponse.hpp
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
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_ATOMIC_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    pthread
)