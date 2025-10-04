INCLUDE(FetchContent)

IF(NOT TARGET meshoptimizer)
	FetchContent_Declare(meshoptimizer_source
		GIT_REPOSITORY https://github.com/zeux/meshoptimizer
		GIT_TAG "v0.25"
	) # or whatever tag you want

	FetchContent_GetProperties(meshoptimizer_source)


	IF(NOT meshoptimizer_source)
		FetchContent_Populate(meshoptimizer_source)

		ADD_SUBDIRECTORY(${meshoptimizer_source_SOURCE_DIR} ${meshoptimizer_source_BINARY_DIR} EXCLUDE_FROM_ALL)

	ELSE()
		MESSAGE( WARNING "Could not find meshoptimizer source code")
	ENDIF()
ENDIF()

