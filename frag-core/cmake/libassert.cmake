INCLUDE(FetchContent)

FetchContent_Declare(libassert_source
	GIT_REPOSITORY https://github.com/jeremy-rifkin/libassert
	GIT_TAG v2.1.4
)

FetchContent_GetProperties(libassert_source)

IF(NOT libassert_source)
	FetchContent_Populate(libassert_source)
	ADD_SUBDIRECTORY(${libassert_source_SOURCE_DIR} ${libassert_source_BINARY_DIR} EXCLUDE_FROM_ALL)
ELSE()
  MESSAGE( WARNING "Could not find libassert source code")
ENDIF()


