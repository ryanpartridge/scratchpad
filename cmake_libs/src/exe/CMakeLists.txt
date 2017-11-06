cmake_minimum_required(VERSION 3.5)

project(dep_tester)

add_executable(${PROJECT_NAME} dep_tester.cpp)
target_link_libraries(${PROJECT_NAME} foo baz)

#get_target_property(EXE_SHARED_LINK_INTERFACE ${PROJECT_NAME} INTERFACE_LINK_LIBRARIES)
#message(STATUS "Link interface libraries for ${PROJECT_NAME}: ${EXE_SHARED_LINK_INTERFACE}")
#get_target_property(EXE_SHARED_LINK ${PROJECT_NAME} LINK_LIBRARIES)
#message(STATUS "Link libraries for ${PROJECT_NAME}: ${EXE_SHARED_LINK}")
