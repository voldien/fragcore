FILE(GLOB SOURCE_PHYSIC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB HEADER_PHYSIC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)


# Library target.
ADD_LIBRARY(fragcore-physic-interface SHARED ${SOURCE_PHYSIC_FILES} ${HEADER_PHYSIC_FILES})
TARGET_LINK_LIBRARIES(fragcore-physic-interface fragcore) #
TARGET_COMPILE_DEFINITIONS(fragcore-physic-interface PRIVATE FRAG_CORE_INTERNAL_IMP=1)
TARGET_INCLUDE_DIRECTORIES(fragcore-physic-interface PUBLIC
											$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
											$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

ADD_DEPENDENCIES(fragcore-physic-interface fragcore )

IF (BUILD_SHARED_LIBS AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	SET_PROPERTY(TARGET fragcore-physic-interface PROPERTY POSITION_INDEPENDENT_CODE ON)
ENDIF()

#
IF(BUILD_WITH_INSTALL)
    INSTALL(TARGETS fragcore-physic-interface DESTINATION lib COMPONENT ${LIBFRAGCORE_COMPONENT})
ENDIF()

