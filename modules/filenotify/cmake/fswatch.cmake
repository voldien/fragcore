INCLUDE(FetchContent)

FetchContent_Declare(fswatch
        GIT_REPOSITORY https://github.com/emcrisostomo/fswatch.git
		GIT_TAG "1.17.1")

FetchContent_GetProperties(fswatch)

IF(NOT fswatch_POPULATED)
	FetchContent_Populate(fswatch)
	ADD_SUBDIRECTORY(${fswatch_SOURCE_DIR} ${fswatch_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()