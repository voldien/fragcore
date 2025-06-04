INCLUDE(FetchContent)

FetchContent_Declare(mesh_generator
	GIT_REPOSITORY https://github.com/voldien/generator
	GIT_TAG "master") # or whatever tag you want

FetchContent_GetProperties(mesh_generator)

IF(NOT mesh_generator_POPULATED)
	FetchContent_Populate(mesh_generator)
	SET(GENERATOR_USE_GLM ON CACHE INTERNAL "Use glm" FORCE)
	ADD_DEFINITIONS(-DGLM_FORCE_CXX17 -DGLM_FORCE_INTRINSICS)
	ADD_SUBDIRECTORY(${mesh_generator_SOURCE_DIR} ${mesh_generator_BINARY_DIR} EXCLUDE_FROM_ALL)

ENDIF()
