cmake_minimum_required(VERSION 3.22)
project(recursive_variant)

set(CMAKE_CXX_STANDARD 11)

set(BOOST_COMPONENTS
    system
)

set(BOOST_ROOT "/home/rpartridge/boost/1.81.0")
set(Boost_USE_STATIC_LIBS "OFF")
set(Boost_NO_SYSTEM_PATHS "ON")

find_package(Boost "1.81" REQUIRED COMPONENTS ${BOOST_COMPONENTS})

set(SOURCE_FILES
    recursive_variant.cpp
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
    ${Boost_SYSTEM_LIBRARY}
)
