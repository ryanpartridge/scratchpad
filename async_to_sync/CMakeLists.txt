cmake_minimum_required(VERSION 3.16)
project(async_to_sync)

set(CMAKE_CXX_STANDARD 11)

set(BOOST_VERSION_MAJOR 1)
set(BOOST_VERSION_MINOR 74)
set(BOOST_VERSION_SUBMINOR 0)
set(BOOST_ROOT $ENV{HOME}/boost/${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR}.${BOOST_VERSION_SUBMINOR})
set(Boost_NO_SYSTEM_PATHS "ON")
#set(Boost_ADDITIONAL_VERSIONS ${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR}.${BOOST_VERSION_SUBMINOR})
set(BOOST_COMPONENTS
    context
    coroutine
    system
    thread
)
find_package(Boost ${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR}.${BOOST_VERSION_SUBMINOR} EXACT REQUIRED
    COMPONENTS
    ${BOOST_COMPONENTS}
)

set(SOURCE_FILES
    async_to_sync.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${Boost_SYSTEM_LIBRARY}
    PRIVATE ${Boost_THREAD_LIBRARY}
    PRIVATE ${Boost_COROUTINE_LIBRARY}
    PRIVATE ${Boost_CONTEXT_LIBRARY}
    PRIVATE pthread
)
