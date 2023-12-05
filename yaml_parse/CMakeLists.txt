cmake_minimum_required(VERSION 3.22.1)
project(yaml_parse)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(RYML_VERSION "VERSION/TAG of RapidYAML library to include" "0.5.0")

include(FetchContent)
FetchContent_Declare(ryml
    GIT_REPOSITORY https://github.com/biojppm/rapidyaml.git
    GIT_TAG ${RYML_VERSION}
    GIT_SHALLOW FALSE
)
FetchContent_MakeAvailable(ryml)

set(SOURCE_FILES
    yaml_parse.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    ryml::ryml
)
