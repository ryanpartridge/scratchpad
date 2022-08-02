cmake_minimum_required(VERSION 3.22)
project(timer_expire)

set(CMAKE_CXX_STANDARD 11)

set(BOOST_COMPONENTS
    chrono
    system
    thread
)

set(BOOST_ROOT "/home/rpartridge/boost/1.75.0")
set(Boost_USE_STATIC_LIBS "OFF")
set(Boost_NO_SYSTEM_PATHS "ON")

find_package(Boost "1.75" REQUIRED COMPONENTS ${BOOST_COMPONENTS})

set(SOURCE_FILES
    timer_expire.cpp
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
    ${Boost_CHRONO_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    pthread
)
