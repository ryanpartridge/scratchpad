cmake_minimum_required(VERSION 3.5)

project(bar)

# create the library varieties all at once
c4_add_libraries(bar Bar.cpp Bar.hpp)

# add the include files
c4_target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

# add bar as a library depenedency
c4_target_link_libraries(${PROJECT_NAME} ZLIB::ZLIB)

# use an object library to only compile the sources once
#add_library(${PROJECT_NAME}_object OBJECT Bar.cpp Bar.hpp)
#target_include_directories(${PROJECT_NAME}_object PUBLIC ${PROJECT_SOURCE_DIR})
#add_libraries_from_object(${PROJECT_NAME} ZLIB::ZLIB)
