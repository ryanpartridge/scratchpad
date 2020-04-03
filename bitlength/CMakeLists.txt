cmake_minimum_required(VERSION 3.5.1)
project(bitlength)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    bitlength.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

#set(CMAKE_C_FLAGS "-m32")
#set(CMAKE_CXX_FLAGS "-m32")