INCLUDE(FetchContent)

FetchContent_Declare(infoware
        GIT_REPOSITORY https://github.com/voldien/infoware.git
		GIT_TAG "arm-improvements") 

FetchContent_GetProperties(infoware)

IF(NOT infoware)
	FetchContent_Populate(infoware)
	ADD_SUBDIRECTORY(${infoware_SOURCE_DIR} ${infoware_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()


