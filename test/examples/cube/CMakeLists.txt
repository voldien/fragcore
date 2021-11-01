# Source files for the gl bunny program.
FILE(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB SOURCE_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/../common/*.cpp)
FILE(GLOB SOURCE_COMMON_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/../common/*.h)


# Executable OpenGL hpm benchmark.
ADD_EXECUTABLE(cube-example ${SOURCE_FILES} ${SOURCE_COMMON} ${SOURCE_COMMON_HEADER})
TARGET_LINK_LIBRARIES(cube-example PUBLIC fragcore fragcore-render-interface PRIVATE ${SDL2_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(cube-example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/)
ADD_DEPENDENCIES(cube-example fragcore)

