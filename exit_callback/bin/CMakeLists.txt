project(exit_callback)

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
    exit_callback.cpp
)

set(HEADER_FILES
    IExitPlugin.hpp
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
    ${Boost_FILESYSTEM_LIBRARY}
)
