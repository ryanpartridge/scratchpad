cmake_minimum_required(VERSION 3.5)

project(baz)

# create the library varieties all at once
c4_add_libraries(baz Baz.cpp Baz.hpp)

# add the include files
c4_target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

# add bar as a library depenedency
c4_target_link_libraries(${PROJECT_NAME} PUBLIC bar)

# use an object library to only compile the sources once
#add_library(${PROJECT_NAME}_object OBJECT Baz.cpp Baz.hpp)
#target_include_directories(${PROJECT_NAME}_object PUBLIC ${PROJECT_SOURCE_DIR})
#target_include_directories(${PROJECT_NAME}_object PUBLIC $<TARGET_PROPERTY:bar_object,INTERFACE_INCLUDE_DIRECTORIES>)
