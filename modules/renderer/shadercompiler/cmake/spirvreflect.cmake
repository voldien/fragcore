INCLUDE(FetchContent)

FetchContent_Declare(spirv_reflect_source
	GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Reflect.git
	GIT_TAG "vulkan-sdk-1.4.304.1"
)

FetchContent_GetProperties(spirv_reflect_source)

IF(NOT spirv_reflect_source)
	FetchContent_Populate(spirv_reflect_source)

    OPTION(SPIRV_REFLECT_EXECUTABLE OFF)
    OPTION(SPIRV_REFLECT_STATIC_LIB ON)
	SET(SPIRV_REFLECT_STATIC_LIB ON)
	ADD_SUBDIRECTORY(${spirv_reflect_source_SOURCE_DIR} ${spirv_reflect_source_BINARY_DIR} EXCLUDE_FROM_ALL)

	SET(SPIRV_REFLECT_STATIC_LIB ON)

ELSE()
	MESSAGE(WARNING "Could not find SPIRV-Cross source code")
ENDIF()
