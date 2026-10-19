INCLUDE(FetchContent)

FetchContent_Declare(alpaca_source
	GIT_REPOSITORY https://github.com/p-ranav/alpaca.git
	GIT_TAG v0.3.0
) # or whatever tag you want

FetchContent_GetProperties(alpaca_source)

IF(NOT alpaca_source)
	FetchContent_Populate(alpaca_source)
	ADD_SUBDIRECTORY(${alpaca_source_SOURCE_DIR} ${alpaca_source_BINARY_DIR} EXCLUDE_FROM_ALL)
ELSE()
  MESSAGE( WARNING "Could not find alpaca source code")
ENDIF()