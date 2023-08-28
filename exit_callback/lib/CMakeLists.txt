project(exitlib)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    DumbExitPlugin.cpp
)

set(HEADER_FILES
    DumbExitPlugin.hpp
)

add_library(${PROJECT_NAME} SHARED
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../bin
)
