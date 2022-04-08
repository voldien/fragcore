# INCLUDE(ExternalProject)

# SET(FSWATCH_DIR ${CMAKE_CURRENT_BINARY_DIR}/fswatch)
# SET(FSWATCH_BIN ${CMAKE_CURRENT_BINARY_DIR}/libfswatch)
# SET(FSWATCH_STATIC_LIB ${FSWATCH_BIN}/lib/fswatch.a)
# SET(FSWATCH_INCLUDES ${FSWATCH_BIN}/include)

# FILE(MAKE_DIRECTORY ${FSWATCH_INCLUDES})

# ExternalProject_Add(
# 	libfswatch
# 	PREFIX ${FSWATCH_DIR}
# 	#INSTALL_DIR ${FSWATCH_BIN}
# 	GIT_REPOSITORY "https://github.com/emcrisostomo/fswatch.git"
# 	GIT_TAG "1.16.0+cmake"
# 	UPDATE_DISCONNECTED True
# 	EXCLUDE_FROM_ALL True
# 	INSTALL_COMMAND make install DESTDIR=${FSWATCH_BIN}
# 	UPDATE_COMMAND ""
# 	PATCH_COMMAND ""
# 	TEST_COMMAND ""
# 	CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX=${FSWATCH_BIN}
# 	COMMENT "Building fswatch"
# 	STEP_TARGETS build
# )

# ADD_LIBRARY(fswatch STATIC IMPORTED GLOBAL)

# ADD_DEPENDENCIES(fswatch libfswatch)

#SET_TARGET_PROPERTIES(fswatch PROPERTIES IMPORTED_LOCATION ${FSWATCH_STATIC_LIB})
#SET_TARGET_PROPERTIES(fswatch PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${FSWATCH_INCLUDES})


INCLUDE(FetchContent)

FetchContent_Declare(fswatch
        GIT_REPOSITORY https://github.com/emcrisostomo/fswatch.git
		GIT_TAG "1.16.0+cmake") # or whatever tag you want

FetchContent_GetProperties(fswatch)

IF(NOT matplotplusplus_POPULATED)
	FetchContent_Populate(fswatch)
	ADD_SUBDIRECTORY(${fswatch_SOURCE_DIR} ${fswatch_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()		