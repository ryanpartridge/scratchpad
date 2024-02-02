cmake_minimum_required(VERSION 3.22)

project(future_work)

set(CMAKE_CXX_STANDARD 11)

set(BOOST_ROOT "/home/rpartridge/boost/1.81.0")
set(Boost_USE_STATIC_LIBS "OFF")
set(Boost_NO_SYSTEM_PATHS "ON")
set(BOOST_COMPONENTS
    filesystem
    system
    thread
)

find_package(Boost "1.81.0" REQUIRED COMPONENTS ${BOOST_COMPONENTS})

set(SOURCE_FILES
    future_work.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_THREAD_LIBRARY}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE BOOST_THREAD_PROVIDES_FUTURE
)
