# Source files for the gl bunny program.
FILE(GLOB AUDIO_EXAMPLE_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB SOURCE_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/../common/*.cpp)
FILE(GLOB SOURCE_COMMON_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/../common/*.h)


ADD_EXECUTABLE(audio-example ${AUDIO_EXAMPLE_SOURCE_FILES} ${SOURCE_COMMON} ${SOURCE_COMMON_HEADER})
TARGET_LINK_LIBRARIES(audio-example PRIVATE fragcore fragcore-audio-interface fragcore-aal )
TARGET_INCLUDE_DIRECTORIES(audio-example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common/ )
ADD_DEPENDENCIES(audio-example fragcore fragcore-audio-interface )
