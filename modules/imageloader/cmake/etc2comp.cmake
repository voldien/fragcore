INCLUDE(FetchContent)

FetchContent_Declare(etc2comp_source
	GIT_REPOSITORY https://github.com/google/etc2comp
	GIT_TAG "master"
) # or whatever tag you want

FetchContent_GetProperties(etc2comp_source)

IF(NOT etc2comp_source)
	FetchContent_Populate(etc2comp_source)
	ADD_SUBDIRECTORY(${etc2comp_source_SOURCE_DIR} ${etc2comp_source_BINARY_DIR} EXCLUDE_FROM_ALL)
  
ELSE()
  MESSAGE( WARNING "Could not find etc2comp source code")
ENDIF()
