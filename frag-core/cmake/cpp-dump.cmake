INCLUDE(FetchContent)

FetchContent_Declare(cpp_dump_source
	GIT_REPOSITORY https://github.com/philip82148/cpp-dump.git
	GIT_TAG v0.7.0
) # or whatever tag you want

FetchContent_GetProperties(cpp_dump_source)

IF(NOT cpp_dump_source)
	FetchContent_Populate(cpp_dump_source)
	ADD_SUBDIRECTORY(${cpp_dump_source_SOURCE_DIR} ${cpp_dump_source_BINARY_DIR} EXCLUDE_FROM_ALL)
ELSE()
  MESSAGE( WARNING "Could not find cpp-dump source code")
ENDIF()