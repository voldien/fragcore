FILE(GLOB IO_SERIAL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB IO_SERIAL_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
INCLUDE(libserialport)

# Library target.
ADD_LIBRARY(fragcore-io-serial ${IO_SERIAL_SOURCE_FILES} ${IO_SERIAL_HEADER_FILES})
TARGET_LINK_LIBRARIES(fragcore-io-serial PUBLIC fragcore serialport) #
TARGET_COMPILE_DEFINITIONS(fragcore-io-serial PRIVATE FRAG_CORE_INTERNAL_IMP=1)

IF (BUILD_SHARED_LIBS AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	SET_PROPERTY(TARGET fragcore-io-serial PROPERTY POSITION_INDEPENDENT_CODE ON)
ENDIF()

#
TARGET_INCLUDE_DIRECTORIES(fragcore-io-serial PUBLIC
							${CMAKE_CURRENT_SOURCE_DIR}
							$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
							$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> )
ADD_DEPENDENCIES(fragcore-io-serial fragcore serialport)

# Sample Program
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/samples)

#
IF(BUILD_WITH_INSTALL)
    INSTALL(TARGETS fragcore-io-serial DESTINATION lib COMPONENT ${LIBFRAGCORE_COMPONENT})
ENDIF()
