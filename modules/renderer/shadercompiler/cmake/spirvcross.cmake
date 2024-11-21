INCLUDE(FetchContent)

FetchContent_Declare(spirv_cross_source
	GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross
	GIT_TAG "vulkan-sdk-1.3.296.0"
)

FetchContent_GetProperties(spirv_cross_source)

IF(NOT spirv_cross_source)
	FetchContent_Populate(spirv_cross_source)

	ADD_SUBDIRECTORY(${spirv_cross_source_SOURCE_DIR} ${spirv_cross_source_BINARY_DIR} EXCLUDE_FROM_ALL)

	SET_PROPERTY(TARGET spirv-cross-glsl spirv-cross-core spirv-cross-cpp spirv-cross-reflect spirv-cross-util PROPERTY CXX_STANDARD 17)

ELSE()
	MESSAGE(WARNING "Could not find SPIRV-Cross source code")
ENDIF()
