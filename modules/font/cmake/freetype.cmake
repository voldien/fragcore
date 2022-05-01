INCLUDE(FetchContent)

FetchContent_Declare(freetype
        GIT_REPOSITORY https://github.com/freetype/freetype
		GIT_TAG "VER-2-12-0") # or whatever tag you want

FetchContent_GetProperties(freetype)

IF(NOT freetype_POPULATED)
	FetchContent_Populate(freetype)
	ADD_SUBDIRECTORY(${freetype_SOURCE_DIR} ${freetype_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()		
