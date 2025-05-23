CMAKE_MINIMUM_REQUIRED(VERSION 3.9..3.20)
PROJECT(FragCore CXX C)

# -----------------------------------------------------------------------------
# Set policy

# see "cmake --help-policy CMP0003"
# So library linking is more sane
CMAKE_POLICY(SET CMP0003 NEW)

# So syntax problems are errors
CMAKE_POLICY(SET CMP0010 NEW)

# Input directories must have CMakeLists.txt
CMAKE_POLICY(SET CMP0014 NEW)

# Silence draco warning on macOS, new policy works fine.
IF(POLICY CMP0068)
	CMAKE_POLICY(SET CMP0068 NEW)
ENDIF()

# find_package() uses <PackageName>_ROOT variables.
IF(POLICY CMP0074)
	CMAKE_POLICY(SET CMP0074 NEW)
ENDIF()

# Check if project is a standalone and not part of another project.
IF(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	SET(FRAGCORE_PROJECT_ROOT TRUE)
ENDIF()

# Use GNUInstallDirs for Unix predefined directories
INCLUDE(GNUInstallDirs)


SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)

# ####################################
# Options
# ####################################
OPTION(BUILD_SHARED_LIBS "Build package with shared libraries." ON)

#
OPTION(BUILD_WITH_EXTERNAL_PROJECT "Enable external project build." ON)
OPTION(BUILD_WITH_INSTALL "Will installs the target associated with the project." ON)

#
OPTION(BUILD_OPT_BUILD_PACKAGES "Set to ON to generate CPack configuration files and packaging targets" OFF)

# Debug
OPTION(BUILD_WITH_COVERAGE "Enable profiling and coverage report analysis." OFF)
OPTION(BUILD_WITH_PROFILER "Enable profiling." OFF)
OPTION(BUILD_WITH_UBSAN "Enable Undefined Behavior Sanitizer." OFF)
OPTION(BUILD_WITH_ASAN "Enable AddressSanitizer." OFF)


#
OPTION(BUILD_WITH_TEST "Enable Testing." OFF)
OPTION(BUILD_WITH_DOCS "Build documentation." OFF)
OPTION(BUILD_WITH_SAMPLES "Build sample codes" OFF)

# Build external interface plugin.
OPTION(BUILD_MODULES "Build module plugins" OFF)
OPTION(BUILD_WITH_OPENMP "Enable OpenMP." OFF)

# Specific Modules.
OPTION(BUILD_EXTERNAL_RENDERER "Building external renderer libraries" ON)
OPTION(BUILD_EXTERNAL_AUDIO "Building external audio libraries" ON)
OPTION(BUILD_EXTERNAL_PHYSIC "Building external physic libraries" ON)

# Override the option if root directory. This implies this is the development project.
IF(FRAGCORE_PROJECT_ROOT)
	SET(BUILD_MODULES ON)
	SET(BUILD_WITH_TEST ON)
	SET(BUILD_WITH_SAMPLES ON)
ENDIF()

# Shared or Static library.
IF(NOT BUILD_SHARED_LIBS)
	SET(CMAKE_EXE_LINKER_FLAGS "-static")
	MESSAGE(STATUS "Static libraries enabled.")
	SET(LINK_SEARCH_START_STATIC TRUE)
ELSE()
	MESSAGE(STATUS "Shared libraries enabled.")
ENDIF(NOT BUILD_SHARED_LIBS)

# SIMD support
OPTION(WITH_CPU_SSE "Enable SIMD instruction if they're detected on the host machine" ON)
MARK_AS_ADVANCED(WITH_CPU_SSE)

#
OPTION(WITH_MEM_JEMALLOC "Enable malloc replacement (http://www.canonware.com/jemalloc)" ON)
MARK_AS_ADVANCED(WITH_MEM_JEMALLOC)

IF(MSVC)
	OPTION(BUILD_INSTALL_PDB
		"Install MSVC debug files."
		ON
	)

	IF(NOT(MSVC_VERSION LESS 1900))
		# Multibyte character set is deprecated since at least MSVC2015 (possibly earlier)
		ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
	ENDIF()
ENDIF()

# Set version of the project.
SET(FRAGCORE_VERSION_MAJOR 0)
SET(FRAGCORE_VERSION_MINOR 12)
SET(FRAGCORE_VERSION_REVISION 0)
SET(FRAGCORE_VERSION_STATE a)
SET(FRAGCORE_VERSION ${FRAGCORE_VERSION_MAJOR}.${FRAGCORE_VERSION_MINOR}${FRAGCORE_VERSION_STATE}${FRAGCORE_VERSION_REVISION})
SET(FRAGCORE_PROJECT_VERSION "${FRAGCORE_VERSION}")

# Set predefined macro for version.
ADD_DEFINITIONS(-DFRAGCORE_VERSION="${FRAGCORE_VERSION}")

# ############################################
# Extract git hash and branch information.
# ############################################
FIND_PACKAGE(Git QUIET)

IF(GIT_FOUND)
	MESSAGE(STATUS "git found: ${GIT_EXECUTABLE} -- ${GIT_VERSION_STRING}")
ENDIF()

IF(NOT FRAG_IGNORE_GIT_HASH)
	# Get the current working branch
	EXECUTE_PROCESS(
		COMMAND git rev-parse --abbrev-ref HEAD
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_BRANCH
		OUTPUT_STRIP_TRAILING_WHITESPACE
		ERROR_QUIET
	)

	# Get the latest abbreviated commit hash of the working branch
	EXECUTE_PROCESS(
		COMMAND git rev-parse --short=8 HEAD
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_COMMIT_HASH
		OUTPUT_STRIP_TRAILING_WHITESPACE
		ERROR_QUIET
	)

	# Extract IF there is a tag associated with current commit
	EXECUTE_PROCESS(
		COMMAND git describe --exact-match --tags 2> /dev/null || git rev-parse --short HEAD
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_TAG
		OUTPUT_STRIP_TRAILING_WHITESPACE
		ERROR_QUIET
	)
ENDIF()

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

SET(LIBFRAGCORE_COMPONENT "libfragcore{FRAGCORE_VERSION_MAJOR}.${FRAGCORE_VERSION_MINOR}.${FRAGCORE_VERSION_REVISION}")
SET(LIBFRAGCORE-DEV_COMPONENT "libfragcore{FRAGCORE_VERSION_MAJOR}.${FRAGCORE_VERSION_MINOR}.${FRAGCORE_VERSION_REVISION}-dev")

# Project uses C++17
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

# ################################
# Inlucde external resources.
# ################################
INCLUDE(magicEnum)

# Compiler toolchain
IF(CMAKE_COMPILER_IS_GNUCC)
	OPTION(WITH_LINKER_GOLD "Use ld.gold linker which is usually faster than ld.bfd" ON)
	MARK_AS_ADVANCED(WITH_LINKER_GOLD)
	OPTION(WITH_LINKER_LLD "Use ld.lld linker which is usually faster than ld.gold" OFF)
	mark_as_advanced(WITH_LINKER_LLD)
ENDIF()

FIND_PACKAGE(SSE)
FIND_PACKAGE(AVX)

IF(WITH_MEM_JEMALLOC)
	FIND_PACKAGE(JeMalloc)
ENDIF()

IF(BUILD_WITH_OPENMP)
	FIND_PACKAGE(OpenMP QUIET)
ENDIF()

# GCC compiler flag options.
IF((CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX) AND NOT CMAKE_COMPILER_IS_MINGW)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wextra -Wall")
	SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wextra -Wall")


	IF(CMAKE_BUILD_TYPE STREQUAL "Release")
		MESSAGE(STATUS "Compile for release.")
		ADD_DEFINITIONS(-DNDEBUG=1)
		ADD_DEFINITIONS(-O3)
		SET(CMAKE_RELEASE TRUE)
		SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
	ELSE()
		MESSAGE(STATUS "Compile for debug.")
		ADD_DEFINITIONS(-D_DEBUG)
		ADD_DEFINITIONS(-g3 -O0)
		SET(CMAKE_DEBUG TRUE)
		ADD_DEFINITIONS(-D_GLIBCXX_ASSERTIONS)
	ENDIF()

	#
	IF(BUILD_WITH_COVERAGE)
		MESSAGE(STATUS "Building with coverage")
		SET(GCC_COVERAGE_COMPILE_FLAGS "--coverage -fprofile-arcs -ftest-coverage")
		SET(GCC_COVERAGE_LINK_FLAGS "-lgcov")
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
		SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
	ENDIF()

	#
	IF(BUILD_WITH_PROFILER)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
		SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
	ENDIF()

	IF(BUILD_WITH_ASAN)
		MESSAGE(STATUS "AddressSanitizer enabled")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
		SET(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
	ENDIF()

	#
	IF(BUILD_WITH_UBSAN)
		MESSAGE(STATUS "Undefined Behavior sanitizer enabled")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined,shift,shift-exponent,integer-divide-by-zero,unreachable,vla-bound,null,return,signed-integer-overflow,bounds,float-divide-by-zero,float-cast-overflow,nonnull-attribute,returns-nonnull-attribute,bool,enum,vptr,pointer-overflow,builtin -fno-sanitize-recover=all")
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined,shift,shift-exponent,integer-divide-by-zero,unreachable,vla-bound,null,return,signed-integer-overflow,bounds,float-divide-by-zero,float-cast-overflow,nonnull-attribute,returns-nonnull-attribute,bool,enum,vptr,pointer-overflow,builtin -fno-sanitize-recover=all")
		SET(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
	ENDIF()

ELSEIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -fno-strict-aliasing -Wall -Wno-long-long ${CMAKE_CXX_FLAGS}")
	SET(CMAKE_C_FLAGS "-fno-strict-aliasing ${CMAKE_C_FLAGS}")

	#
	IF(BUILD_WITH_ASAN)
		MESSAGE(STATUS "AddressSanitizer enabled")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
		SET(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
	ENDIF()

ELSEIF(MSVC)
	# Enable multi-core compilation with MSVC
	IF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") # clang-cl
		ADD_COMPILE_OPTIONS(/bigobj /W4 /WX)
	ELSE() # msvc
		ADD_COMPILE_OPTIONS(/MP /bigobj /W4 /WX)
	ENDIF()

	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG /Zi /Od")
ELSEIF(CMAKE_COMPILER_IS_MINGW)
	SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -fno-strict-aliasing -Wall -Wno-long-long -Wa,-mbig-obj ${CMAKE_CXX_FLAGS}")
	SET(CMAKE_C_FLAGS "-fno-strict-aliasing ${CMAKE_C_FLAGS}")
	ADD_DEFINITIONS(-U__STRICT_ANSI__)
ELSE()
	MESSAGE(ERROR "No Compiler Found")
ENDIF()

IF(WITH_CPU_SSE)
	INCLUDE(FindSSE)
	#
	IF(FOUND_FindSEE)
		TEST_SSE_SUPPORT(COMPILER_SSE_FLAG COMPILER_SSE2_FLAG)
	ENDIF()

ELSE()
	MESSAGE(STATUS "SSE and SSE2 optimizations are DISABLED!")
	SET(COMPILER_SSE_FLAG)
	SET(COMPILER_SSE2_FLAG)
ENDIF()

#
IF(UNIX AND NOT APPLE)
	IF(NOT WITH_CXX11_ABI)
		STRING(APPEND PLATFORM_CFLAGS " -D_GLIBCXX_USE_CXX11_ABI=0")
		STRING(APPEND PLATFORM_CXXFLAGS " -D_GLIBCXX_USE_CXX11_ABI=0")
	ENDIF()
ENDIF()

# Cache these to allow the user to override them manually.
SET(FRAGCORE_LIB_INSTALL_DIR "lib" CACHE STRING
	"Path the built library files are installed to.")
SET(FRAGCORE_INCLUDE_INSTALL_DIR "fragcore-core" CACHE STRING
	"Path the header files are installed to.")
SET(FRAGCORE_BIN_INSTALL_DIR "bin" CACHE STRING
	"Path the tool executables are installed to.")

IF(UNIX)
	SET(FRAGCORE_LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
	SET(FRAGCORE_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
	SET(FRAGCORE_BIN_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})
ELSE()
	# Cache these to allow the user to override them on non-Unix platforms
	SET(FRAGCORE_LIB_INSTALL_DIR "lib" CACHE STRING
		"Path the built library files are installed to.")
	SET(FRAGCORE_INCLUDE_INSTALL_DIR "include" CACHE STRING
		"Path the header files are installed to.")
	SET(FRAGCORE_BIN_INSTALL_DIR "bin" CACHE STRING
		"Path the tool executables are installed to.")

	SET(CMAKE_INSTALL_FULL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/${FRAGCORE_INCLUDE_INSTALL_DIR})
	SET(CMAKE_INSTALL_FULL_LIBDIR ${CMAKE_INSTALL_PREFIX}/${FRAGCORE_LIB_INSTALL_DIR})
	SET(CMAKE_INSTALL_FULL_BINDIR ${CMAKE_INSTALL_PREFIX}/${FRAGCORE_BIN_INSTALL_DIR})
ENDIF()

FIND_PACKAGE(PkgConfig QUIET)

# ########################################
# Main core library.
# ########################################
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/frag-core)

# Testing and assertion and other.
IF(BUILD_WITH_TEST)
	ENABLE_TESTING()
ENDIF()

# #########################################################
# Seperate Modules, based on the core library.
# #########################################################
IF(BUILD_MODULES)
	# Image Loader
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/image)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/image/imageloader)

	# Windows
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/window)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/window/sdl)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/window/glfw3)

	# Rendering interfaces.
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/renderer/)
	#
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/renderer/opengl/)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/renderer/vulkan/)

	# Build FragCore Audio modules
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/audio)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/audio/openal/)

	# 
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/geometryutil/)

	# Build FragCore Physic modules
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/physic/)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/physic/bullet/)

	# Database
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/db/sqlite)

	# File watcher.
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/filenotify)

	# IO
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/IO/curl)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/IO/can)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/IO/modbus)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/IO/SerialIO)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/IO/compression)

	#
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/modules/taskscheduler/TaskSch)

ENDIF(BUILD_MODULES)

# #########################################
# Add Tests if Enabled for core library.
# ########################################
IF(NOT ANDROID)
	IF(BUILD_WITH_TEST)
		ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/test/)
	ENDIF()
ENDIF()

# #######################################
# Generate sample executables.
# #######################################
IF(BUILD_WITH_SAMPLES)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/test/examples)
ENDIF()

# ######################################
# Generate documentations.
# ######################################
IF(BUILD_WITH_DOCS)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/docs/)
ENDIF(BUILD_WITH_DOCS)

# Only UNIX systems.
IF(UNIX)
	SET(FRAG_LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
	SET(FRAG_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
	SET(FRAG_BIN_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})

ELSEIF(WIN32)
ELSE()
	# Cache these to allow the user to override them on non-Unix platforms
	SET(FRAG_LIB_INSTALL_DIR "lib" CACHE STRING
		"Path the built library files are installed to.")
	SET(FRAG_INCLUDE_INSTALL_DIR "include" CACHE STRING
		"Path the header files are installed to.")
	SET(FRAG_BIN_INSTALL_DIR "bin" CACHE STRING
		"Path the tool executables are installed to.")

	SET(CMAKE_INSTALL_FULL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/${FRAG_INCLUDE_INSTALL_DIR})
	SET(CMAKE_INSTALL_FULL_LIBDIR ${CMAKE_INSTALL_PREFIX}/${FRAG_LIB_INSTALL_DIR})
	SET(CMAKE_INSTALL_FULL_BINDIR ${CMAKE_INSTALL_PREFIX}/${FRAG_BIN_INSTALL_DIR})
ENDIF()

IF(WIN32)
	IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
		SET(BIN_DIR "${PROJECT_SOURCE_DIR}/bin64/")
		SET(LIB_DIR "${PROJECT_SOURCE_DIR}/lib64/")
	else()
		SET(BIN_DIR "${PROJECT_SOURCE_DIR}/bin32/")
		SET(LIB_DIR "${PROJECT_SOURCE_DIR}/lib32/")
	ENDIF()

	IF(MSVC_TOOLSET_VERSION)
		set(MSVC_PREFIX "vc${MSVC_TOOLSET_VERSION}")
	ELSE()
		IF(MSVC12)
		ELSEIF(MSVC14)
		ELSEIF(MSVC15)
		ENDIF(MSVC12)
	ENDIF()

	IF(MSVC12 OR MSVC14 OR MSVC15)
	ENDIF(MSVC12 OR MSVC14 OR MSVC15)
ENDIF(WIN32)

# #############################################
# Create install package.
# #############################################
IF(BUILD_WITH_INSTALL)
	IF(CMAKE_CPACK_COMMAND AND UNIX AND BUILD_OPT_BUILD_PACKAGES)
		# Packing information
		SET(CPACK_PACKAGE_NAME "fragcore${FRAG_VERSION_MAJOR}.${VERSION_MINOR}")
		SET(CPACK_PACKAGE_CONTACT "" CACHE STRING "Package maintainer and PGP signer.")
		SET(CPACK_PACKAGE_VENDOR "https://github.com/voldien/fragcore")
		SET(CPACK_PACKAGE_DISPLAY_NAME "Frag ${VERSION}")
		SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY " -  ${VERSION}")
		SET(CPACK_PACKAGE_VERSION "${VERSION}.${PACKAGE_VERSION}")
		SET(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
		SET(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
		SET(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
		SET(CPACK_PACKAGE_INSTALL_DIRECTORY "frag${VERSION_MAJOR}.${VERSION_MINOR}")
		SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

		STRING(TOUPPER ${LIBFRAGCORE_COMPONENT} "LIBFRAGCORE_COMPONENT_UPPER")
		STRING(TOUPPER ${LIBFRAGCORE-DEV_COMPONENT} "LIBFRAGCORE-DEV_COMPONENT_UPPER")

		#
		SET(CPACK_COMPONENT_FRAGCORE-BIN_DISPLAY_NAME "tools")
		SET(CPACK_COMPONENT_FRAGCORE-BIN_DEPENDS "${LIBFRAGCORE_COMPONENT}")
		SET(CPACK_COMPOENTT_${LIBFRACORE_COMPONENT_UPPER}_DISPLAY_NAME "")
		SET(CPACK_COMPOENTT_${LIBFRACORE-DEV_COMPONENT_UPPER}_DISPLAY_NAME "")
		SET(CPACK_COMPONENT_${LIBFRACORE-DEV_COMPONENT_UPPER}_DEPENDS $ "{LIBFRAGCORE_COMPONENT}")
		SET(CPACK_COMPOENTT_${LIBFRACORE-DEV_COMPONENT_UPPER}_DISPLAY_NAME "")
		SET(CPACK_COMPONENT_${LIBFRAGCORE_COMPONENT_UPPER}_DISPLAY_NAME "libraries")
		SET(CPACK_COMPONENT_${LIBFRAGCORE-DEV_COMPONENT_UPPER}_DISPLAY_NAME "common headers and installs")
		SET(CPACK_COMPONENT_${LIBFRAGCORE-DEV_COMPONENT_UPPER}_DEPENDS $ "{LIBFRAGCORE_COMPONENT}")
		SET(CPACK_COMPONENT_LIBFRAGCORE-DEV_DISPLAY_NAME "${CPACK_COMPONENT_${LIBFRAGCORE-DEV_COMPONENT}_DISPLAY_NAME}")
		SET(CPACK_COMPONENT_LIBFRAGCORE-DEV_DEPENDS "${LIBFRAGCORE-DEV_COMPONENT}")
		SET(CPACK_DEBIAN_BUILD_DEPENDS debhelper cmake zlib1g-dev pkg-config)

		# debian
		SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
		SET(CPACK_DEBIAN_CMAKE_OPTIONS "")
		SET(CPACK_DEBIAN_PACKAGE_SECTION "libs")
		SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_COMPONENTS_ALL}")
		SET(CPACK_DEBIAN_PACKAGE_SUGGESTS)
		SET(cPACK_DEBIAN_PACKAGE_NAME "fragcore")
		SET(CPACK_DEBIAN_PACKAGE_REMOVE_SOURCE_FILES build doc obj samples packaging utils)
		SET(CPACK_DEBIAN_PACKAGE_SOURCE_COPY svn export --force)
		SET(CPACK_DEBIAN_CHANGELOG)
		execute_process(COMMAND lsb_release -is
			OUTPUT_VARIABLE _lsb_distribution OUTPUT_STRIP_TRAILING_WHITESPACE
			RESULT_VARIABLE _lsb_release_failed)
		SET(CPACK_DEBIAN_DISTRIBUTION_NAME ${_lsb_distribution} CACHE STRING "Name of the distrubiton")
		STRING(TOLOWER ${CPACK_DEBIAN_DISTRIBUTION_NAME} CPACK_DEBIAN_DISTRIBUTION_NAME)

		IF(${CPACK_DEBIAN_DISTRIBUTION_NAME} STREQUAL "ubuntu")
			SET(CPACK_DEBIAN_DISTRIBUTION_RELEASES lucid maverick natty oneiric precise CACHE STRING "Release code-names of the distrubiton release")
		ENDIF()

		SET(DPUT_HOST "" CACHE STRING "PPA repository to upload the debian sources")

		INCLUDE(CPack)
		INCLUDE(DebSourcePPA)
	ENDIF()
ENDIF()

# ###################################
# Final verbose message status.
# ###################################
MESSAGE(STATUS "")
MESSAGE(STATUS "These are the configured paths:")
MESSAGE(STATUS "  PREFIX: ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "  LIB_DIR: ${LIB_DIR}")
MESSAGE(STATUS "  BIN_DIR: ${BIN_DIR}")
MESSAGE(STATUS "  MENU_DIR: ${MENU_DIR}")
MESSAGE(STATUS "")
MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "Target bitness: ${CMAKE_SIZEOF_VOID_P}*8")
MESSAGE(STATUS "")
