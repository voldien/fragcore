cmake_minimum_required(VERSION 3.6)
project(fragengine)


set(FRAGCORE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# Get sources
file(GLOB_RECURSE SOURCES ${FRAGCORE_ROOT_DIR}/*.cpp**)
file(GLOB_RECURSE HEADERS ${FRAGCORE_ROOT_DIR}/*.h**)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES} ${HEADERS})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE ${FRAGCORE_ROOT_DIR})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${log-lib} )

