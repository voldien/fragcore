INCLUDE(FetchContent)

FetchContent_Declare(libffmpeg_source
	GIT_REPOSITORY https://github.com/FFmpeg/FFmpeg.git
	GIT_TAG n3.0
) # or whatever tag you want

FetchContent_GetProperties(libffmpeg_source)

IF(NOT libffmpeg_source)
	FetchContent_Populate(libffmpeg_source)


INCLUDE(ExternalProject)

SET(FFMPEG_DIR ${libffmpeg_source_SOURCE_DIR})
SET(FFMPEG_BIN ${libffmpeg_source_BINARY_DIR})
SET(FFMPEG_LIB ${FFMPEG_BIN}/lib/libffmpeg.a)
SET(FFMPEG_INCLUDE_DIRS ${FFMPEG_BIN}/include)

FILE(MAKE_DIRECTORY ${FFMPEG_INCLUDE_DIRS})

ExternalProject_Add(
  libFFMPEG
  PREFIX ${FFMPEG_BIN}
  SOURCE_DIR ${FFMPEG_DIR}
  CONFIGURE_COMMAND ${FFMPEG_DIR}/configure --srcdir=${FFMPEG_DIR} --prefix=${FFMPEG_BIN} --shared  CFLAGS=-fPIC CXXFLAGS=-fPIC
  BUILD_COMMAND make
  UPDATE_DISCONNECTED True
  COMMENT           "Building ffmpeg"
  BUILD_BYPRODUCTS ${FFMPEG_LIB}
)

ADD_LIBRARY(ffmpeg STATIC IMPORTED)

ADD_DEPENDENCIES(ffmpeg libFFMPEG)

SET_TARGET_PROPERTIES(ffmpeg PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB})
SET_TARGET_PROPERTIES(ffmpeg PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${FFMPEG_INCLUDE_DIRS})

ENDIF()