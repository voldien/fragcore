FILE(GLOB RENDERER_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB RENDERER_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# Library target.
ADD_LIBRARY(fragcore-render-interface ${RENDERER_SOURCE_FILES} ${RENDERER_HEADER_FILES})
TARGET_LINK_LIBRARIES(fragcore-render-interface PUBLIC fragcore fragcore-image fragcore-window)
TARGET_COMPILE_DEFINITIONS(fragcore-render-interface PRIVATE FRAG_CORE_INTERNAL_IMP=1)

#
TARGET_INCLUDE_DIRECTORIES(fragcore-render-interface PUBLIC
											${CMAKE_CURRENT_SOURCE_DIR}
											$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
											$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

ADD_DEPENDENCIES(fragcore-render-interface fragcore )

IF (BUILD_SHARED_LIBS AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	SET_PROPERTY(TARGET fragcore-render-interface PROPERTY POSITION_INDEPENDENT_CODE ON)
ENDIF()

#
IF(BUILD_WITH_INSTALL)
    INSTALL(TARGETS fragcore-render-interface DESTINATION lib COMPONENT ${LIBFRAGCORE_COMPONENT})
ENDIF()

#####################
#
#####################
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/shadercompiler)