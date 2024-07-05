INCLUDE(FetchContent)

FetchContent_Declare(magic_enum_source
	GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
	GIT_TAG v0.9.5
) # or whatever tag you want

FetchContent_GetProperties(magic_enum_source)

IF(NOT magic_enum_source)
	FetchContent_Populate(magic_enum_source)
	ADD_SUBDIRECTORY(${magic_enum_source_SOURCE_DIR} ${magic_enum_source_BINARY_DIR} EXCLUDE_FROM_ALL)
ELSE()
  MESSAGE( WARNING "Could not find better-enums source code")
ENDIF()


