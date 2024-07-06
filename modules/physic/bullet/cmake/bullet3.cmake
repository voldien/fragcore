INCLUDE(FetchContent)

FetchContent_Declare(libbullet3_source
	GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
	GIT_TAG 3.25
) # or whatever tag you want

FetchContent_GetProperties(libbullet3_source)

IF(NOT libbullet3_source)
	FetchContent_Populate(libbullet3_source)
	ADD_SUBDIRECTORY(${libbullet3_source_SOURCE_DIR} ${libbullet3_source_BINARY_DIR} EXCLUDE_FROM_ALL)
  
ELSE()
  MESSAGE( WARNING "Could not find libbullet3 source code")
ENDIF()
