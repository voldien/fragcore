INCLUDE(FetchContent)

FetchContent_Declare(infoware
        GIT_REPOSITORY https://github.com/ThePhD/infoware.git
		GIT_TAG 2a56cb1c6eb48ef36566f587e207c27af641f0a6) 

FetchContent_GetProperties(infoware)

IF(NOT infoware)
	FetchContent_Populate(infoware)
	SET(INFOWARE_USE_PCIIDS OFF CACHE BOOL "Disable PCI IDS")
	ADD_SUBDIRECTORY(${infoware_SOURCE_DIR} ${infoware_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()


