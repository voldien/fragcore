INCLUDE(FetchContent)

FetchContent_Declare(infoware
        GIT_REPOSITORY https://github.com/ThePhD/infoware.git
		GIT_TAG 79f0aa5864322b041c1660845fdc7f7fce2af69a) 

FetchContent_GetProperties(infoware)

IF(NOT infoware)
	FetchContent_Populate(infoware)
	SET(INFOWARE_USE_PCIIDS OFF CACHE BOOL "Disable PCI IDS")
	ADD_SUBDIRECTORY(${infoware_SOURCE_DIR} ${infoware_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()


