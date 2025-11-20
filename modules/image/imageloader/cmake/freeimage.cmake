
INCLUDE(FetchContent)
IF(NOT TARGET FreeImage::FreeImage)
	FetchContent_Declare(freeimage_source
		GIT_REPOSITORY https://github.com/danoli3/FreeImage.git
		GIT_TAG 8268e809b0827870763444e8a8e58e0a83a733f5
	) # or whatever tag you want

	FetchContent_GetProperties(freeimage_source)


	IF(NOT freeimage_source)
		FetchContent_Populate(freeimage_source)

		SET(BUILD_OPENEXR OFF)

		ADD_SUBDIRECTORY(${freeimage_source_SOURCE_DIR} ${freeimage_source_BINARY_DIR} EXCLUDE_FROM_ALL)
		TARGET_INCLUDE_DIRECTORIES(FreeImage PUBLIC $<BUILD_INTERFACE:${freeimage_source_SOURCE_DIR}/Source>)

	ELSE()
		MESSAGE( WARNING "Could not find FreeImage source code")
	ENDIF()
ENDIF()

