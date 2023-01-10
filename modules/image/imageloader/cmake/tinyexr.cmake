
#https://github.com/syoyo/tinyexr

INCLUDE(FetchContent)

FetchContent_Declare(tinyexr_source
	GIT_REPOSITORY https://github.com/syoyo/tinyexr
	GIT_TAG "v1.0.1"
) # or whatever tag you want

FetchContent_GetProperties(tinyexr_source)

IF(NOT tinyexr_source)
	FetchContent_Populate(tinyexr_source)
	ADD_SUBDIRECTORY(${tinyexr_source_SOURCE_DIR} ${tinyexr_source_BINARY_DIR} EXCLUDE_FROM_ALL)
  
ELSE()
  MESSAGE( WARNING "Could not find tinyexr source code")
ENDIF()
