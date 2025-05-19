INCLUDE(FetchContent)

FetchContent_Declare(infoware
        GIT_REPOSITORY https://github.com/ThePhD/infoware.git
		GIT_TAG bc54e8dcfb6387193289c702e5be18aae73f3084) 

FetchContent_GetProperties(infoware)

IF(NOT infoware)
	FetchContent_Populate(infoware)
	SET(INFOWARE_USE_PCIIDS OFF CACHE BOOL "Disable PCI IDS")
	ADD_SUBDIRECTORY(${infoware_SOURCE_DIR} ${infoware_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()


