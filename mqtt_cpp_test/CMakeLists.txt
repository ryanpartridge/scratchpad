cmake_minimum_required(VERSION 3.22.1)
project(mqtt_cpp_test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(PkgConfig REQUIRED)
pkg_check_modules(SSL REQUIRED IMPORTED_TARGET openssl>=3.0)

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
    mqtt_cpp_test.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE MQTT_USE_TLS
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/mqtt_cpp/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE PkgConfig::SSL
    PRIVATE Boost::thread
    PRIVATE pthread
)
