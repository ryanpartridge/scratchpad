cmake_minimum_required(VERSION 3.5)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

find_package(ZLIB REQUIRED)

# Takes an OBJECT library and creates INTERFACE,
# SHARED, and STATIC versions of the library
macro(c4_add_libraries object_lib_target)
    # get the base library name
    if(${object_lib_target} MATCHES "^.+_object$")
        string(REGEX REPLACE "^(.*)_object$" "\\1" LIB_BASE_NAME ${object_lib_target})
    else()
        set(LIB_BASE_NAME ${object_lib_target})
    endif()
    #message(STATUS "library base name: ${LIB_BASE_NAME}")

    # create the object library and exclude from all
    add_library(${LIB_BASE_NAME}_object OBJECT ${ARGN})
    set_target_properties(${LIB_BASE_NAME}_object PROPERTIES EXCLUDE_FROM_ALL TRUE)

    # create the interface library (cannot be excluded from all - no target is built)
    add_library(${LIB_BASE_NAME}_interface INTERFACE)
    set_target_properties(${LIB_BASE_NAME}_interface
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_BASE_NAME}_object,INTERFACE_INCLUDE_DIRECTORIES>
    )

    # create the shared library and exclude from all
    add_library(${LIB_BASE_NAME} SHARED $<TARGET_OBJECTS:${LIB_BASE_NAME}_object>)
    set_target_properties(${LIB_BASE_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    set_target_properties(${LIB_BASE_NAME}
        PROPERTIES
        INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_BASE_NAME}_object,INCLUDE_DIRECTORIES>
        INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_BASE_NAME}_object,INTERFACE_INCLUDE_DIRECTORIES>
    )

    # create the static library and exclude from all
    add_library(${LIB_BASE_NAME}_static STATIC $<TARGET_OBJECTS:${LIB_BASE_NAME}_object>)
    set_target_properties(${LIB_BASE_NAME}_static PROPERTIES EXCLUDE_FROM_ALL TRUE)
    set_target_properties(${LIB_BASE_NAME}_static
        PROPERTIES
        INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_BASE_NAME}_object,INCLUDE_DIRECTORIES>
        INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_BASE_NAME}_object,INTERFACE_INCLUDE_DIRECTORIES>
    )
endmacro()

macro(c4_sync_include_directories target)
    set_target_properties(${target}_interface
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${target}_object,INTERFACE_INCLUDE_DIRECTORIES>
    )

    set_target_properties(${target}
        PROPERTIES
        INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${target}_object,INCLUDE_DIRECTORIES>
        INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${target}_object,INTERFACE_INCLUDE_DIRECTORIES>
    )

    set_target_properties(${target}_static
        PROPERTIES
        INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${target}_object,INCLUDE_DIRECTORIES>
        INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${target}_object,INTERFACE_INCLUDE_DIRECTORIES>
    )
endmacro()

# Add include dependencies to a set of libraries
macro(c4_target_include_directories target)
    #message(STATUS "adding include directories ${ARGN} to library: ${target}")
    if(NOT TARGET ${target}_object)
        message(FATAL_ERROR "c4_target_include_directories can only be invoked on a target created by c4_target_add_libraries")
    endif()
    set(INC_DEP_TYPE "PUBLIC")
    foreach(INC_ARG ${ARGN})
        if(${INC_ARG} MATCHES "PUBLIC|PRIVATE|INTERFACE")
            set(INC_DEP_TYPE ${INC_ARG})
        else()
            #message(STATUS "[${target}] include directory ${INC_ARG} (${INC_DEP_TYPE})")
            target_include_directories(${target}_object ${INC_DEP_TYPE} ${INC_ARG})
            set(INC_DEP_TYPE "PUBLIC")
        endif()
    endforeach()
    c4_sync_include_directories(${target})
endmacro()

# Add library dependencies to a set of libraries -- the
# target must have been created with c4_target_add_libraries
macro(c4_target_link_libraries target)
    # add the link dependencies
    #message(STATUS "adding link dependency to library: ${target}")
    if(NOT TARGET ${target}_object)
        message(FATAL_ERROR "c4_target_link_libraries can only be invoked on a target created by c4_target_add_libraries")
    endif()
    set(LIB_DEP_TYPE "PUBLIC")
    set(LIB_DEP_OPTION "")
    foreach(LIB_ARG ${ARGN})
        if(${LIB_ARG} MATCHES "PUBLIC|PRIVATE|INTERFACE")
            set(LIB_DEP_TYPE ${LIB_ARG})
        elseif (${LIB_ARG} MATCHES "debug|DEBUG|general|GENERAL|optimized|OPTIMIZED")
            set(LIB_DEP_OPTION ${LIB_ARG})
        else()
            message(STATUS "[${target}] link dependency ${LIB_ARG} (${LIB_DEP_TYPE})")
            target_link_libraries(${target} ${LIB_DEP_TYPE} ${LIB_DEP_OPTION} ${LIB_ARG})

            # add the interface library to the target's static library (if applicable)
            if(TARGET ${LIB_ARG}_interface)
                message(STATUS "[${target}]_static link dependency ${LIB_ARG}_interface (${LIB_DEP_TYPE})")
                target_link_libraries(${target}_static ${LIB_DEP_TYPE} ${LIB_ARG}_interface)
            endif()

            # add to interface library
            if(${LIB_DEP_TYPE} MATCHES "PUBLIC|INTERFACE")
                target_link_libraries(${target}_interface INTERFACE ${LIB_ARG})
            endif()

            # if the dependency is a target, add the include directories to the object library
            if(TARGET ${LIB_ARG}_object)
                if(${LIB_DEP_TYPE} MATCHES "PUBLIC")
                    set_property(TARGET ${target}_object APPEND PROPERTY INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_ARG}_object,INTERFACE_INCLUDE_DIRECTORIES>)
                    set_property(TARGET ${target}_object APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_ARG}_object,INTERFACE_INCLUDE_DIRECTORIES>)
                elseif(${LIB_DEP_TYPE} MATCHES "INTERFACE")
                    set_property(TARGET ${target}_object APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_ARG}_object,INTERFACE_INCLUDE_DIRECTORIES>)
                elseif(${LIB_DEP_TYPE} MATCHES "PRIVATE")
                    set_property(TARGET ${target}_object APPEND PROPERTY INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${LIB_ARG}_object,INTERFACE_INCLUDE_DIRECTORIES>)
                endif()
            endif()
            set(LIB_DEP_TYPE "PUBLIC")
            set(LIB_DEP_OPTION "")
        endif()
    endforeach()

    # make sure the include directories are in sync
    c4_sync_include_directories(${target})
endmacro()

add_subdirectory(bar)
add_subdirectory(foo)
add_subdirectory(baz)
add_subdirectory(exe)
