cmake_minimum_required(VERSION 3.5)

project(baz)

# use an object library to only compile the sources once
add_library(${PROJECT_NAME}_object OBJECT Baz.cpp Baz.hpp)
target_include_directories(${PROJECT_NAME}_object PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME}_object PUBLIC $<TARGET_PROPERTY:bar_object,INTERFACE_INCLUDE_DIRECTORIES>)

add_libraries_from_object(baz bar)

# create an interface library
#add_library(${PROJECT_NAME}_interface INTERFACE)
#set_target_properties(${PROJECT_NAME}_interface
#    PROPERTIES
#    INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${PROJECT_NAME}_object,INTERFACE_INCLUDE_DIRECTORIES>
#)

# create the shared library
#add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:${PROJECT_NAME}_object>)
#set_target_properties(${PROJECT_NAME}
#    PROPERTIES
#    INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${PROJECT_NAME}_object,INCLUDE_DIRECTORIES>
#    INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${PROJECT_NAME}_object,INTERFACE_INCLUDE_DIRECTORIES>
#    EXCLUDE_FROM_ALL TRUE
#)
#target_link_libraries(${PROJECT_NAME} PUBLIC bar)

# create the static library
#add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}_object>)
#set_target_properties(${PROJECT_NAME}_static
#    PROPERTIES
#    INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${PROJECT_NAME}_object,INCLUDE_DIRECTORIES>
#    INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${PROJECT_NAME}_object,INTERFACE_INCLUDE_DIRECTORIES>
#    EXCLUDE_FROM_ALL TRUE
#)
#target_link_libraries(${PROJECT_NAME}_static PUBLIC bar_interface)
