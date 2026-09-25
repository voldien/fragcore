INCLUDE(FetchContent)

FetchContent_Declare(libsigcplusplus_source
	GIT_REPOSITORY https://github.com/libsigcplusplus/libsigcplusplus.git
	GIT_TAG 3.8.1
)

FetchContent_GetProperties(libsigcplusplus_source)

IF(NOT libsigcplusplus_source)
	FetchContent_Populate(libsigcplusplus_source)
	ADD_SUBDIRECTORY(${libsigcplusplus_source_SOURCE_DIR} ${libsigcplusplus_source_BINARY_DIR} EXCLUDE_FROM_ALL)
ELSE()
  MESSAGE( WARNING "Could not find libsigcplusplus source code")
ENDIF()
