cmake_minimum_required(VERSION 3.22.1)
project(cmake_defines)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(C4_FOO OFF CACHE BOOL "Set to ON to enable FOO feature.")
message(STATUS "Value of C4_FOO: ${C4_FOO}")

set(SOURCE_FILES
    cmake_defines.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

if(C4_FOO)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE C4_FOO
    )
endif()
