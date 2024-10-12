INCLUDE(FetchContent)

FetchContent_Declare(marl_source
	GIT_REPOSITORY https://github.com/google/marl.git
	GIT_TAG 690889fbb816a409b4fcd4bdf60e32a4deebeb08
) 

FetchContent_GetProperties(marl_source)

IF(NOT marl_source)
	FetchContent_Populate(marl_source)
	ADD_SUBDIRECTORY(${marl_source_SOURCE_DIR} ${marl_source_BINARY_DIR} EXCLUDE_FROM_ALL)
ELSE()
	MESSAGE( WARNING "Could not find marl source code")
ENDIF()


