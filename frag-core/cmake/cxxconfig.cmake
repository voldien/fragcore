INCLUDE(FetchContent)

FetchContent_Declare(cxxconfig_source
	GIT_REPOSITORY https://github.com/voldien/cxx-config.git
	GIT_TAG develop
) # or whatever tag you want

FetchContent_GetProperties(cxxconfig_source)

IF(NOT cxxconfig_source)
	FetchContent_Populate(cxxconfig_source)
	ADD_SUBDIRECTORY(${cxxconfig_source_SOURCE_DIR} ${cxxconfig_source_BINARY_DIR} EXCLUDE_FROM_ALL)
ELSE()
  MESSAGE( WARNING "Could not find cxxconfig source code")
ENDIF()
