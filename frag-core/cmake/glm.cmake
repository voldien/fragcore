INCLUDE(FetchContent)

FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/icaven/glm
		GIT_TAG dca38025fba63bb9284023e6de55f756b9e37cec)

FetchContent_GetProperties(glm)

IF(NOT glm)
	FetchContent_Populate(glm)

	SET(GLM_ENABLE_CXX_17 ON)
	ADD_SUBDIRECTORY(${glm_SOURCE_DIR} ${glm_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()


