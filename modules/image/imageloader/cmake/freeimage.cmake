
INCLUDE(FetchContent)
IF(NOT TARGET FreeImage::FreeImage)
	FetchContent_Declare(freeimage_source
		GIT_REPOSITORY https://github.com/danoli3/FreeImage.git
		GIT_TAG "master"
	) # or whatever tag you want

	FetchContent_GetProperties(freeimage_source)


	IF(NOT freeimage_source)
		FetchContent_Populate(freeimage_source)

		SET(BUILD_OPENEXR OFF)

		ADD_SUBDIRECTORY(${freeimage_source_SOURCE_DIR} ${freeimage_source_BINARY_DIR} EXCLUDE_FROM_ALL)

	ELSE()
		MESSAGE( WARNING "Could not find FreeImage source code")
	ENDIF()
ENDIF()

