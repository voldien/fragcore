

#SET(CMAKE_SYSTEM_NAME Android)
ENABLE_LANGUAGE(ASM_NASM)

# Source files.
FILE(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB HEADER_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

ADD_LIBRARY( fragcore-android-native-lib ${SOURCE_FILES} ${HEADER_FILES} )
ADD_DEPENDENCIES(fragcore-android-native-lib fragcore)

#TODO add target for generating the java library.
