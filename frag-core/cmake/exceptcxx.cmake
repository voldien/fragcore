INCLUDE(FetchContent)

FetchContent_Declare(exceptcxx_source
	GIT_REPOSITORY https://github.com/voldien/exceptcxx
	GIT_TAG a3ad6a99a15b9c8394f0fa10ecb3e3ff78e0ed39
)

FetchContent_GetProperties(exceptcxx_source)

IF(NOT exceptcxx_source)
	FetchContent_Populate(exceptcxx_source)
	ADD_SUBDIRECTORY(${exceptcxx_source_SOURCE_DIR} ${exceptcxx_source_BINARY_DIR} EXCLUDE_FROM_ALL)
ELSE()
  MESSAGE( WARNING "Could not find exceptcxx source code")
ENDIF()
