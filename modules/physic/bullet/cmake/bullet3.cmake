INCLUDE(FetchContent)

FetchContent_Declare(libbullet3_source
	GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
	GIT_TAG 3.25
) # or whatever tag you want

FetchContent_GetProperties(libbullet3_source)

IF(NOT libbullet3_source)
	FetchContent_Populate(libbullet3_source)

	SET(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "")
	SET(ENABLE_VHACD OFF CACHE BOOL "")
	SET(BULLET2_MULTITHREADING ON CACHE BOOL "")
	SET(BUILD_CPU_DEMOS OFF CACHE BOOL "")
	SET(BUILD_ENET OFF CACHE BOOL "")
	SET(BUILD_CLSOCKET OFF CACHE BOOL "")
	SET(BUILD_UNIT_TESTS OFF CACHE BOOL "")
	SET(BULLET2_USE_OPEN_MP_MULTITHREADING ON CACHE BOOL "")
	ADD_SUBDIRECTORY(${libbullet3_source_SOURCE_DIR} ${libbullet3_source_BINARY_DIR} EXCLUDE_FROM_ALL)
  
ELSE()
  MESSAGE( WARNING "Could not find libbullet3 source code")
ENDIF()
