cmake_minimum_required(VERSION 3.15)
project(fmt_test CXX)

find_package(fmt REQUIRED)

add_executable(fmt_test src/fmt_test.cpp)

target_link_libraries(fmt_test fmt::fmt)

install(TARGETS fmt_test DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )

