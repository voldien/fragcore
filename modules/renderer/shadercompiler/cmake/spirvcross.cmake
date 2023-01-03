INCLUDE(FetchContent)

FetchContent_Declare(spirv_cross_source
	GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross
	GIT_TAG sdk-1.3.236.0
)

FetchContent_GetProperties(spirv_cross_source)

IF(NOT spirv_cross_source)
	FetchContent_Populate(spirv_cross_source)

	SET(SPIRV_CROSS_ENABLE_GLSL ON)
	SET(SPIRV_CROSS_SKIP_INSTALL ON)
	SET(SPIRV_CROSS_FORCE_PIC ON)
	SET(SPIRV_CROSS_STATIC ON)
	SET(SPIRV_CROSS_CLI OFF)
	
	SET(SPIRV_CROSS_SHARED OFF)
	
	
	ADD_SUBDIRECTORY(${spirv_cross_source_SOURCE_DIR} ${spirv_cross_source_BINARY_DIR} EXCLUDE_FROM_ALL)

	set_property(TARGET spirv-cross-glsl spirv-cross-core spirv-cross-cpp spirv-cross-reflect spirv-cross-util PROPERTY CXX_STANDARD 17)
  
ELSE()
  MESSAGE( WARNING "Could not find SPIRV-Cross source code")
ENDIF()
