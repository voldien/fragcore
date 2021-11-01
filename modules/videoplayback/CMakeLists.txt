
#
IF(PKG_CONFIG_FOUND)
	MESSAGE(STATUS "Found pkg-config: ${PKG_CONFIG_EXECUTABLE} -- version ${PKG_CONFIG_VERSION_STRING}.")
	# Find library and compiling options.
	#TODO add logic for handling if any of the pkg fails.
	PKG_CHECK_MODULES(AVFORMAT QUIET libavformat)
	PKG_CHECK_MODULES(AVCODEC QUIET libavcodec)
	PKG_CHECK_MODULES(AVUTIL QUIET libavutil)
	PKG_CHECK_MODULES(SWSCALE QUIET libswscale)
ELSE()
	MESSAGE(ERROR "")
	# TODO add logic for handling if pkg-config not available.
ENDIF()



FILE(GLOB VIDEO_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB VIDEO_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# Core library for adding support for plugin.
ADD_LIBRARY(fragcore-videoplayback ${VIDEO_SOURCE_FILES} ${VIDEO_HEADER_FILES})
ADD_DEPENDENCIES( fragcore-videoplayback fragcore fragcore-render-interface fragcore-audio-interface )

#
TARGET_COMPILE_DEFINITIONS(fragcore-videoplayback PRIVATE FRAG_CORE_INTERNAL_IMP=1)

#
SET_TARGET_PROPERTIES(fragcore-videoplayback PROPERTIES
		COMPILE_FLAGS " ${AVFORMAT_CFLAGS_OTHER} ${AVCODEC_CFLAGS_OTHER}
		${AVUTIL_CFLAGS_OTHER} ${SWSCALE_CFLAGS_OTHER} ")

#
TARGET_LINK_LIBRARIES(fragcore-videoplayback PUBLIC fragcore fragcore-render-interface fragcore-audio-interface PRIVATE ${AVFORMAT_LIBRARIES} ${AVCODEC_LIBRARIES} ${SWSCALE_LIBRARIES} ${AVUTIL_LIBRARIES} )

#
TARGET_INCLUDE_DIRECTORIES(fragcore  PUBLIC
											$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
											$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
