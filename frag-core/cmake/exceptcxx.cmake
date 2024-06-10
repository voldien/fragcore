INCLUDE(FetchContent)

FetchContent_Declare(exceptcxx_source
	GIT_REPOSITORY https://github.com/voldien/exceptcxx
	GIT_TAG 35126d6b59ef48a73c904dbfab3e719c95bdc1a3
) # or whatever tag you want

FetchContent_GetProperties(exceptcxx_source)

IF(NOT exceptcxx_source)
	FetchContent_Populate(exceptcxx_source)
	ADD_SUBDIRECTORY(${exceptcxx_source_SOURCE_DIR} ${exceptcxx_source_BINARY_DIR} EXCLUDE_FROM_ALL)
ELSE()
  MESSAGE( WARNING "Could not find exceptcxx source code")
ENDIF()
