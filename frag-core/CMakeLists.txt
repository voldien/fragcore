OPTION(BUILTIN_NANOSVG "Build with nanosvg" ON)
OPTION(BUILTIN_EIGEN "Build with Eigen" ON)
OPTION(BUILTIN_OPEN_SIMPLEX_NOISE "Build with open Simple Noise" ON)
OPTION(BUILTIN_PCG_C_BASIC "Build with PCG-C Basic" ON)
OPTION(BUILTIN_MISC "" ON)

SET(THIRDPARTY_INCLUDE_DIR "" PARENT_SCOPE)
SET(THIRDPARTY_SOURCE "" PARENT_SCOPE)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

########################
# External libraries.
########################
INCLUDE(infoware)
INCLUDE(taskscheduler)
IF(NOT TARGET spdlog)
	INCLUDE(spdlog)
ENDIF()

IF(NOT TARGET cxxexcept)
	INCLUDE(exceptcxx)
ENDIF()

IF(BUILTIN_OPEN_SIMPLEX_NOISE)
	SET(OPEN_SIMPLEX_NOISE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/open-simplex-noise-in-c)
	FILE(GLOB OPEN_SIMPLEX_NOISE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/open-simplex-noise-in-c/*.c )
ENDIF()

IF(BUILTIN_PCG_C_BASIC)
	SET(PCG_C_BASIC_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/pcg-c-basic)
	FILE(GLOB PCG_C_BASIC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/pcg-c-basic/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/extern/pcg-c-basic/*.c )
ENDIF()

# Find all required libaries.
FIND_PACKAGE(ZLIB QUIET)
FIND_PACKAGE(Backtrace QUIET)
FIND_PACKAGE(LZO QUIET)
FIND_PACKAGE(OpenSSL REQUIRED)
# Find all optional libaries.
FIND_PACKAGE(JeMalloc QUIET)

#
IF(PKG_CONFIG_FOUND)
	MESSAGE(STATUS "Found pkg-config: ${PKG_CONFIG_EXECUTABLE} -- version ${PKG_CONFIG_VERSION_STRING}.")
	PKG_CHECK_MODULES(ZIP QUIET libzip)
ELSE()
	MESSAGE(ERROR "Could not find PKG COnfig")
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
ENDIF()

IF(LZO_FOUND)

ENDIF()

FIND_PACKAGE(Eigen3 3.3 NO_MODULE QUIET)
IF(EIGEN3_FOUND)
	MESSAGE(STATUS "Using eigen3 version.")
ELSE()

ENDIF()

# Define all file lists.
FILE(GLOB SOURCE_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/threading/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/math/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/datastructure/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/io/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/core/network/*.cpp
		)

#
FILE(GLOB HEADER_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
						${CMAKE_CURRENT_SOURCE_DIR}/include/Core/*.h
						${CMAKE_CURRENT_SOURCE_DIR}/include/Compression/*.h
						${CMAKE_CURRENT_SOURCE_DIR}/include/DataStructure/*.h
						${CMAKE_CURRENT_SOURCE_DIR}/include/Math/*.h
						${CMAKE_CURRENT_SOURCE_DIR}/include/Math3D/*.h
						${CMAKE_CURRENT_SOURCE_DIR}/include/Threading/*.h
						${CMAKE_CURRENT_SOURCE_DIR}/include/Networking/*.h
						${CMAKE_CURRENT_SOURCE_DIR}/include/IO/*.h
						)

# Public headers - subset of header files.
FILE(GLOB DEVELOPMENT_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/Core/*.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/Core/IO/*.h)

SET(FRAG_CORE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(FRAG_CORE_INCLUDE_SUB_DIR
				${CMAKE_CURRENT_SOURCE_DIR}/include
				${CMAKE_CURRENT_SOURCE_DIR}/include/Core
				${CMAKE_CURRENT_SOURCE_DIR}/include/Utils)


# Core library for adding support for plugin.
ADD_LIBRARY(fragcore ${SOURCE_FILES} ${HEADER_FILES}
 							${OPEN_SIMPLEX_NOISE_SOURCE} ${PCG_C_BASIC_SOURCE}
							${MISC_SOURCE})
TARGET_COMPILE_FEATURES(fragcore PUBLIC cxx_constexpr cxx_alias_templates cxx_raw_string_literals
cxx_variadic_templates cxx_uniform_initialization cxx_right_angle_brackets cxx_nullptr
cxx_generic_lambdas cxx_override cxx_noexcept cxx_aggregate_default_initializers)
ADD_DEPENDENCIES(fragcore taskSch infoware cxxexcept )

#
TARGET_COMPILE_DEFINITIONS(fragcore PRIVATE FRAG_CORE_INTERNAL_IMP=1 PUBLIC FV_USE_SHARED_POINTER=1)

IF(EIGEN3_FOUND)
	TARGET_COMPILE_DEFINITIONS(fragcore PUBLIC FRAGCORE_USE_EIGEN=1)
ENDIF()
TARGET_COMPILE_FEATURES(fragcore PUBLIC cxx_std_17 cxx_constexpr)

#
TARGET_LINK_LIBRARIES(fragcore PUBLIC Eigen3::Eigen taskSch cxxexcept spdlog_header_only fmt magic_enum PRIVATE stdc++ atomic dl ${ZLIB_LIBRARIES} 
													${ZIP_LIBRARIES} m ${OPENSSL_CRYPTO_LIBRARY} infoware )

#
TARGET_INCLUDE_DIRECTORIES(fragcore PRIVATE ${ZIP_INCLUDE_DIRS}
													 ${OPEN_SIMPLEX_NOISE_INCLUDE_DIRS} ${PCG_C_BASIC_INCLUDE_DIRS} 
													 ${ZLIB_INCLUDE_DIRS} ${ZSTD_INCLUDE_DIRS}
													 ${Backtrace_HEADER}
													 ${infoware_SOURCE_DIR}/include/infoware
										PUBLIC
											$<BUILD_INTERFACE:${FRAG_CORE_INCLUDE}>
											$<INSTALL_INTERFACE:${FRAG_CORE_INCLUDE}>
)

#
IF(OpenMP_FOUND AND OpenMP_CXX_FOUND AND BUILD_WITH_OPENMP)
	TARGET_LINK_LIBRARIES(fragcore PRIVATE m pthread PUBLIC OpenMP::OpenMP_CXX)
	TARGET_COMPILE_OPTIONS(fragcore PUBLIC ${OpenMP_CXX_FLAGS})
ENDIF()


# Enable FPIC if shared library.
IF (BUILD_SHARED_LIBS AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	SET_PROPERTY(TARGET fragcore PROPERTY POSITION_INDEPENDENT_CODE ON)
ENDIF()

# Create package information
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/fragcore.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/fragcore.pc" @ONLY)
# Generate revision for up to date header with version and etc.
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/include/fragcore_revision.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/fragcore_revision.h" @ONLY)
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/include/fragcore_revision.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/include/fragcore_revision.h" @ONLY)

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
