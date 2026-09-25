INCLUDE(FetchContent)

#
FetchContent_Declare(libenvpp_source
	GIT_REPOSITORY https://github.com/ph3at/libenvpp.git
	GIT_TAG v1.5.3
)

FetchContent_GetProperties(libenvpp_source)

IF(NOT libenvpp_source)
	FetchContent_Populate(libenvpp_source)
	ADD_SUBDIRECTORY(${libenvpp_source_SOURCE_DIR} ${libenvpp_source_BINARY_DIR} EXCLUDE_FROM_ALL)
ELSE()
  MESSAGE( WARNING "Could not find libenvpp source code")
ENDIF()
