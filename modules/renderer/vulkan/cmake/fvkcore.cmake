INCLUDE(FetchContent)

FetchContent_Declare(fvkcore_source
	GIT_REPOSITORY https://github.com/voldien/fvkcore.git
	GIT_TAG develop
) # or whatever tag you want

FetchContent_GetProperties(fvkcore_source)

IF(NOT fvkcore_source)
	FetchContent_Populate(fvkcore_source)
	ADD_SUBDIRECTORY(${fvkcore_source_SOURCE_DIR} ${fvkcore_source_BINARY_DIR} EXCLUDE_FROM_ALL)
  
ELSE()
  MESSAGE( WARNING "Could not find fvkcore source code")
ENDIF()
