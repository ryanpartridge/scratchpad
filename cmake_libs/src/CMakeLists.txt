cmake_minimum_required(VERSION 3.5)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

find_package(ZLIB REQUIRED)

macro(add_libraries_from_object OBJECT_LIB_TARGET)
    # get the base library name
    if(${OBJECT_LIB_TARGET} MATCHES "^.+_object$")
        string(REGEX REPLACE "^(.*)_object$" "\\1" LIB_BASE_NAME ${OBJECT_LIB_TARGET})
    else()
        set(LIB_BASE_NAME ${OBJECT_LIB_TARGET})
    endif()
    #message(STATUS "library base name: ${LIB_BASE_NAME}")

    # exclude the object from all
    set_target_properties(${LIB_BASE_NAME}_object PROPERTIES EXCLUDE_FROM_ALL TRUE)

    # create the interface library
    add_library(${LIB_BASE_NAME}_interface INTERFACE)
    set_target_properties(${LIB_BASE_NAME}_interface
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_BASE_NAME}_object,INTERFACE_INCLUDE_DIRECTORIES>
    )

    # create the shared library
    add_library(${LIB_BASE_NAME} SHARED $<TARGET_OBJECTS:${LIB_BASE_NAME}_object>)
    set_target_properties(${LIB_BASE_NAME}
        PROPERTIES
        INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_BASE_NAME}_object,INCLUDE_DIRECTORIES>
        INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_BASE_NAME}_object,INTERFACE_INCLUDE_DIRECTORIES>
        EXCLUDE_FROM_ALL TRUE
    )

    # create the static library
    add_library(${LIB_BASE_NAME}_static STATIC $<TARGET_OBJECTS:${LIB_BASE_NAME}_object>)
    set_target_properties(${LIB_BASE_NAME}_static
        PROPERTIES
        INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_BASE_NAME}_object,INCLUDE_DIRECTORIES>
        INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_BASE_NAME}_object,INTERFACE_INCLUDE_DIRECTORIES>
        EXCLUDE_FROM_ALL TRUE
    )

    # add the link dependencies
    set(LIB_DEP_TYPE "PUBLIC")
    foreach(LIB_ARG ${ARGN})
        if(${LIB_ARG} MATCHES "PUBLIC|PRIVATE|INTERFACE")
            set(LIB_DEP_TYPE ${LIB_ARG})
        else()
            #message(STATUS "[${LIB_BASE_NAME}] link dependency ${LIB_ARG} (${LIB_DEP_TYPE})")
            target_link_libraries(${LIB_BASE_NAME} ${LIB_DEP_TYPE} ${LIB_ARG})

            # only add the interface library to the static library if the target exists
            if(TARGET ${LIB_ARG}_interface)
                target_link_libraries(${LIB_BASE_NAME}_static ${LIB_DEP_TYPE} ${LIB_ARG}_interface)
            endif()
            set(LIB_DEP_TYPE "PUBLIC")
        endif()
    endforeach()
endmacro()

add_subdirectory(exe)
add_subdirectory(foo)
add_subdirectory(baz)
add_subdirectory(bar)
