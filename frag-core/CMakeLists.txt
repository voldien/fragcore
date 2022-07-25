
OPTION(BUILD_WITH_SDL_WINDOW "Use SDL for window mangement." ON)
#OPTION(BUILD_WITH_TASK_SCHEDULER_SCHTASK ON)

OPTION(BUILTIN_NANOSVG "Build with nanosvg" ON)
OPTION(BUILTIN_EIGEN "Build with Eigne" ON)
OPTION(BUILTIN_OPEN_SIMPLEX_NOISE "Build with open Simple Noise" ON)
OPTION(BUILTIN_PCG_C_BASIC "Build with PCG-C Basic" ON)
OPTION(BUILTIN_KISS_FFT "" ON)
OPTION(BUILTIN_MISC "" ON)

SET(THIRDPARTY_INCLUDE_DIR "" PARENT_SCOPE)
SET(THIRDPARTY_SOURCE "" PARENT_SCOPE)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

INCLUDE(infoware)
IF(NOT TARGET spdlog)
	INCLUDE(spdlog)
ENDIF()
IF(NOT TARGET cxxexcept)
	INCLUDE(exceptcxx)
ENDIF()
#Eigen

IF(BUILTIN_OPEN_SIMPLEX_NOISE )
	SET(OPEN_SIMPLEX_NOISE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/open-simplex-noise-in-c)
	FILE(GLOB OPEN_SIMPLEX_NOISE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/open-simplex-noise-in-c/*.c )
ENDIF()

IF(BUILTIN_PCG_C_BASIC)
	SET(PCG_C_BASIC_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/pcg-c-basic)
	FILE(GLOB PCG_C_BASIC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/pcg-c-basic/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/extern/pcg-c-basic/*.c )
ENDIF()

IF(BUILTIN_KISS_FFT)
	SET(KISS_FFT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/kissfft )
	FILE(GLOB KISS_FFT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/kissfft/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/extern/kissfft/*.c)
ENDIF()

# Find all required libaries for the program.
FIND_PACKAGE(ZLIB QUIET)
FIND_PACKAGE(Backtrace QUIET)
FIND_PACKAGE(OpenSSL QUIET)
FIND_PACKAGE(JeMalloc QUIET)
FIND_PACKAGE(LLVM QUIET)
FIND_PACKAGE(LZO QUIET)


#
IF(PKG_CONFIG_FOUND)
	MESSAGE(STATUS "Found pkg-config: ${PKG_CONFIG_EXECUTABLE} -- version ${PKG_CONFIG_VERSION_STRING}.")
	# Find library and compiling options.
	#TODO add logic for handling if any of the pkg fails.
	PKG_CHECK_MODULES(JSONC QUIET json-c)
	PKG_CHECK_MODULES(ZIP QUIET libzip)
ELSE()
	MESSAGE(ERROR "Could not find PKG COnfig")
	# TODO add logic for handling if pkg-config not available.
ENDIF()

IF(ZLIB_FOUND)
	MESSAGE(STATUS "zlib found: ${ZLIB_LIBRARIES} -- ${ZLIB_VERSION_STRING}")
ELSE()

ENDIF()

IF(Backtrace_FOUND)
	MESSAGE(STATUS "backtrace found: ${Backtrace_LIBRARIES} -- ${Backtrace_HEADER}")
ELSE()

ENDIF()

IF(OPENSSL_FOUND)
	MESSAGE(STATUS "OpenSSL found: ${OPENSSL_LIBRARIES} -- ${OPENSSL_VERSION}")
ELSE()

ENDIF()

# Find and include libxml2 for supporting configuration.
FIND_PACKAGE(LibXml2 QUIET)
IF ( LIBXML2_FOUND )
	MESSAGE(STATUS "Using xml2 version ${LIBXML2_VERSION_STRING}.")
ELSE()
ENDIF()

FIND_PACKAGE(JeMalloc QUIET)
IF(JeMalloc_FOUND)

ENDIF()

FIND_PACKAGE(LZO QUIET)
IF(LZO_FOUND)

ENDIF()


FIND_PACKAGE(Eigen3 3.3 NO_MODULE QUIET)
IF(EIGEN3_FOUND)
	MESSAGE(STATUS "Using eigen3 version.")
ENDIF()

# Define all file lists.
FILE(GLOB SOURCE_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/threading/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/math/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/datastructure/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/io/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/network/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/scheduler/*.cpp
		)

#
FILE(GLOB HEADER_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
						${CMAKE_CURRENT_SOURCE_DIR}/include/Utils/*.h)

# Public headers - subset of header files.
FILE(GLOB DEVELOPMENT_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/Core/*.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/Core/event/*.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/Core/IO/*.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/Utils/*.h)

SET(FRAG_CORE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(FRAG_CORE_INCLUDE_SUB_DIR
				${CMAKE_CURRENT_SOURCE_DIR}/include
				${CMAKE_CURRENT_SOURCE_DIR}/include/Core
				${CMAKE_CURRENT_SOURCE_DIR}/include/Utils)


# Core library for adding support for plugin.
ADD_LIBRARY(fragcore ${SOURCE_FILES} ${HEADER_FILES}
 							${OPEN_SIMPLEX_NOISE_SOURCE} ${PCG_C_BASIC_SOURCE}
							${KISS_FFT_SOURCE}
							${MISC_SOURCE})
ADD_DEPENDENCIES(fragcore taskSch infoware cxxexcept cxx-config)

#
TARGET_COMPILE_DEFINITIONS(fragcore PRIVATE FRAG_CORE_INTERNAL_IMP=1)
TARGET_COMPILE_FEATURES(fragcore PUBLIC cxx_std_17 cxx_constexpr)

#
TARGET_LINK_LIBRARIES(fragcore PUBLIC Eigen3::Eigen taskSch cxxexcept spdlog_header_only fmt cxx-config magic_enum PRIVATE stdc++ atomic dl ${ZLIB_LIBRARIES} ${LIBXML2_LIBRARIES} ${FREETYPE_LIBRARIES}
													${ZIP_LIBRARIES} m ${JSONC_LIBRARIES} yaml ${OPENSSL_CRYPTO_LIBRARY} infoware )

#
TARGET_INCLUDE_DIRECTORIES(fragcore PRIVATE ${ZIP_INCLUDE_DIRS} ${JSONC_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR}
													 ${OPEN_SIMPLEX_NOISE_INCLUDE_DIRS} ${PCG_C_BASIC_INCLUDE_DIRS} 
													 ${KISS_FFT_INCLUDE_DIRS}
													 ${ZLIB_INCLUDE_DIRS} ${ZSTD_INCLUDE_DIRS}
													 ${Backtrace_HEADER}
													 ${infoware_SOURCE_DIR}/include/infoware
										PUBLIC
											$<BUILD_INTERFACE:${FRAG_CORE_INCLUDE}>
											$<INSTALL_INTERFACE:${FRAG_CORE_INCLUDE}>
)

# Enable FPIC if shared library.
IF (BUILD_SHARED_LIBS AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	SET_PROPERTY(TARGET fragcore PROPERTY POSITION_INDEPENDENT_CODE ON)
ENDIF()

# Create package information
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/fragcore.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/fragcore.pc" @ONLY)
# Generate revision for up to date header with version and etc.
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/include/fragcore_revision.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/fragcore_revision.h" @ONLY)

# Install
IF(BUILD_WITH_INSTALL)
	INSTALL(TARGETS fragcore DESTINATION lib COMPONENT ${LIBFRAGCORE_COMPONENT})
	INSTALL(TARGETS infoware taskSch DESTINATION lib)

	INSTALL(DIRECTORY ${FRAG_CORE_INCLUDE_SUB_DIR} # source directory
			DESTINATION include/fragcore # target directory
			COMPONENT ${LIBFRAGCORE-DEV_COMPONENT}
			)
	INSTALL( FILES "${CMAKE_CURRENT_BINARY_DIR}/fragcore.pc" DESTINATION ${FRAGCORE_LIB_INSTALL_DIR}/pkgconfig/ COMPONENT ${LIBFRAGCORE-DEV_COMPONENT})
ENDIF()
