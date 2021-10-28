FILE(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB SOURCE_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/../common/*.cpp)
FILE(GLOB SOURCE_COMMON_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/../common/*.h)

# Executable OpenGL hpm benchmark.
ADD_EXECUTABLE(modbus-tcp-sample ${SOURCE_FILES} ${SOURCE_COMMON} ${SOURCE_COMMON_HEADER})
TARGET_LINK_LIBRARIES(modbus-tcp-sample PRIVATE fragcore fragcore-ionet-modbus)
TARGET_INCLUDE_DIRECTORIES(modbus-tcp-sample PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/ )
ADD_DEPENDENCIES(modbus-tcp-sample fragcore fragcore-ionet-modbus )
