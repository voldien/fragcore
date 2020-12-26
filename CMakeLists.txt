CMAKE_MINIMUM_REQUIRED( VERSION 3.11.0 FATAL_ERROR )
PROJECT(fragview VERSION 0.11.1 LANGUAGES CXX C)

# Build shared/dynamic library option
OPTION(BUILD_SHARED_LIBS "Build package with shared libraries." ON)
OPTION(BUILD_WITH_COVERAGE "Enable profiling and coverage report analysis." OFF)
OPTION(BUILD_WITH_PROFILER "Enable profiling." OFF)
OPTION(BUILD_WITH_EXTERNAL_PROJECT "Enable external project build." ON)
OPTION(BUILD_WITH_INSTALL "Disable this if you want to use fragengine as a submodule." ON )
OPTION(BUILD_WITH_UBSAN "" OFF)
OPTION(BUILD_WITH_ASAN "Enable AddressSanitizer." OFF )
OPTION(BUILD_OPT_BUILD_PACKAGES "Set to ON to generate CPack configuration files and packaging targets" OFF)
OPTION(BUILD_WITH_DEBUG_BOUNDSCHECK "Array/Pointer BoundsChecking, buffer overflow protection. Used in debug only." OFF)
option(BUILD_INSTALL_RESOURCES "Installs the Resources directory" ON)
IF(NOT BUILD_SHARED_LIBS)
	SET(CMAKE_EXE_LINKER_FLAGS "-static")
	SET(LINK_SEARCH_START_STATIC TRUE)
ELSE()
	MESSAGE(STATUS "Shared libraries enabled")
ENDIF(NOT BUILD_SHARED_LIBS)

#
OPTION(BUILD_WITH_TEST "Enable Testing." ON)
OPTION(BUILD_WITH_DOCS "Build documentation." OFF)
OPTION(BUILD_WITH_CORE "Build Core" ON)
OPTION(BUILD_WITH_SAMPLES "Build sample codes" ON)

# Build external interface plugin.
OPTION(BUILD_WITH_EXTERNAL_PLUGINS "" ON)
OPTION(BUILD_EXTERNAL_RENDERER "Building external renderer libraries" ON)
OPTION(BUILD_EXTERNAL_AUDIO "Building external audio libraries" ON)
OPTION(BUILD_EXTERNAL_PHYSIC "Building external physic libraries" ON)

IF(MSVC)
  OPTION( BUILD_INSTALL_PDB
	"Install MSVC debug files."
	ON
  )
  IF(NOT (MSVC_VERSION LESS 1900))
	# Multibyte character set is deprecated since at least MSVC2015 (possibly earlier)
	ADD_DEFINITIONS( -DUNICODE -D_UNICODE )
  ENDIF()
ENDIF()


# Set version of the project.
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 11)
SET(VERSION_REVISION 0)
SET(VERSION_STATE a)
SET(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}${VERSION_STATE}${VERSION_REVISION} )
SET(PROJECT_VERSION "${VERSION}")

# Set predefined macro for version.
ADD_DEFINITIONS(-DFV_VERSION="${VERSION}")

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
ENDIF()

# CONFIGURE_FILE(
#   ${CMAKE_CURRENT_LIST_DIR}/revision.h.in
#   ${CMAKE_CURRENT_BINARY_DIR}/revision.h
# )

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules" )
SET(LIBFRAGCORE_COMPONENT "libfragcore{VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}")
SET(LIBFRAGCORE-DEV_COMPONENT "libfragcore{VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}-dev")
SET(LIBFRAGENGINE_COMPONENT "libfragengine{VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}")
SET(LIBFRAGENGINE-DEV_COMPONENT "libfragengine{VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}-dev")

SET(FRAGVIEW_COMPONENT "fragview{VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}")
SET(FRAEDITOR_COMPONENT "frageditor{VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}")

SET(CPACK_COMPONENTS_ALL fragview-bin ${LIBFRAGCORE_COMPONENT} ${LIBFRAGCORE-DEV_COMPONENT}
	${LIBFRAGENGINE_COMPONENT} ${LIBFRAGENGINE-DEV_COMPONENT} $FRAGVIEW_COMPONENT} ${FRAGEDITOR_COMPONENT})
SET(FRAGVIEW_LIBRARY_SUFFIX "" CACHE STRING "Suffix to append to library names")

# macro that sets a default (path) if one wasn't specified
MACRO(SET_PATH variable default)
	IF(NOT ${variable})
		SET(${variable} ${default})
	ENDIF(NOT ${variable})
ENDMACRO(SET_PATH)

#LIB_DIRif (${LAYOUT} MATCHES "home")
SET_PATH( PLUGIN_DIR ${CMAKE_INSTALL_PREFIX}/plugins )
SET_PATH( DATA_DIR ${CMAKE_INSTALL_PREFIX} )
SET_PATH( MAN_DIR ${CMAKE_INSTALL_PREFIX}/man/man6 )
SET_PATH( BIN_DIR ${CMAKE_INSTALL_PREFIX} )
SET_PATH( SYSCONF_DIR ${CMAKE_INSTALL_PREFIX} )
SET_PATH( LIB_DIR ${CMAKE_INSTALL_PREFIX} )
SET_PATH( DOC_DIR ${CMAKE_INSTALL_PREFIX}/doc )
SET_PATH( ICON_DIR ${CMAKE_INSTALL_PREFIX} )
SET_PATH( SVG_DIR ${CMAKE_INSTALL_PREFIX} )
SET_PATH( MENU_DIR ${CMAKE_INSTALL_PREFIX} )
SET_PATH( EXAMPLE_CONF_DIR ${CMAKE_INSTALL_PREFIX} )
#elseif (${LAYOUT} MATCHES "fhs")

#-std=c++1z
SET(CMAKE_REQUIRED_FLAGS "-std=c++17 -std=c11 -std=c++1z")
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_C_EXTENSIONS OFF)

# GCC compiler flag options.
IF((CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX) AND NOT CMAKE_COMPILER_IS_MINGW)
	IF (BUILD_SHARED_LIBS AND CMAKE_SIZEOF_VOID_P EQUAL 8) # -fPIC is only required
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	ENDIF()

	# 
	ADD_DEFINITIONS(-Wall -w -fpermissive) 
	ADD_DEFINITIONS( -export-dynamic -rdynamic -lstdc++fs -std=c++17 )

	IF (CMAKE_BUILD_TYPE STREQUAL "Release")
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
		SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
		SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
		SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
		SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
	ENDIF()

	#
	IF(BUILD_WITH_PROFILER)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
		SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
	ENDIF()
ELSEIF ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
  SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -fno-strict-aliasing -Wall -Wno-long-long ${CMAKE_CXX_FLAGS}" )
  SET(CMAKE_C_FLAGS "-fno-strict-aliasing ${CMAKE_C_FLAGS}")
ELSEIF(MSVC)
	# Enable multi-core compilation with MSVC
	IF( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" ) # clang-cl
		ADD_COMPILE_OPTIONS(/bigobj /W4 /WX )
	ELSE() # msvc
		ADD_COMPILE_OPTIONS(/MP /bigobj /W4 /WX)
	ENDIF()

	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG /Zi /Od")
ELSEIF ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
	SET(CMAKE_CXX_FLAGS "-g -fvisibility=hidden -fno-strict-aliasing -Wall -Wno-long-long ${CMAKE_CXX_FLAGS}" )
	SET(CMAKE_C_FLAGS "-fno-strict-aliasing ${CMAKE_C_FLAGS}")
ELSEIF( CMAKE_COMPILER_IS_MINGW )
	SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -fno-strict-aliasing -Wall -Wno-long-long -Wa,-mbig-obj ${CMAKE_CXX_FLAGS}")
	SET(CMAKE_C_FLAGS "-fno-strict-aliasing ${CMAKE_C_FLAGS}")
	ADD_DEFINITIONS( -U__STRICT_ANSI__ )
ELSE()

ENDIF()


#
IF ( IOS)
  IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fembed-bitcode -Og")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fembed-bitcode -Og")
  ELSE()
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fembed-bitcode -O3")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fembed-bitcode -O3")
	# Experimental for pdb generation
  ENDIF()
ENDIF()

#
IF (BUILD_WITH_UBSAN)
	MESSAGE(STATUS "Undefined Behavior sanitizer enabled")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined,shift,shift-exponent,integer-divide-by-zero,unreachable,vla-bound,null,return,signed-integer-overflow,bounds,float-divide-by-zero,float-cast-overflow,nonnull-attribute,returns-nonnull-attribute,bool,enum,vptr,pointer-overflow,builtin -fno-sanitize-recover=all")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined,shift,shift-exponent,integer-divide-by-zero,unreachable,vla-bound,null,return,signed-integer-overflow,bounds,float-divide-by-zero,float-cast-overflow,nonnull-attribute,returns-nonnull-attribute,bool,enum,vptr,pointer-overflow,builtin -fno-sanitize-recover=all")
ENDIF()

#
IF (BUILD_WITH_ASAN)
	MESSAGE(STATUS "AddressSanitizer enabled")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
ENDIF()

# Cache these to allow the user to override them manually.
SET( FRAGVIEW_LIB_INSTALL_DIR "lib" CACHE STRING
		"Path the built library files are installed to." )
SET( FRAGVIEW_INCLUDE_INSTALL_DIR "fragview-core" CACHE STRING
		"Path the header files are installed to." )
SET( FRAGVIEW_BIN_INSTALL_DIR "bin" CACHE STRING
		"Path the tool executables are installed to." )

# Find all required libaries for the program.
FIND_PACKAGE(ZLIB )
FIND_PACKAGE(Git QUIET)
FIND_PACKAGE(Backtrace)
FIND_PACKAGE(PkgConfig QUIET)
FIND_PACKAGE(OpenSSL)
#TODO relocate to the compute plugin.
FIND_PACKAGE(CUDA QUIET)
IF( UNIX )
  # Use GNUInstallDirs for Unix predefined directories
  INCLUDE(GNUInstallDirs)
ENDIF()

IF(CUDA_FOUND)

ELSE()

ENDIF()

IF(GIT_FOUND)
	MESSAGE(STATUS "git found: ${GIT_EXECUTABLE} -- ${GIT_VERSION_STRING}")
ENDIF()
IF(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	# Update submodules as needed
	OPTION(GIT_SUBMODULE "Check submodules during build" ON)
	IF(GIT_SUBMODULE)
		MESSAGE(STATUS "Submodule update --recursive")
		EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
						RESULT_VARIABLE GIT_SUBMOD_RESULT)
		IF(NOT GIT_SUBMOD_RESULT EQUAL "0")
			MESSAGE(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		ENDIF()

	ENDIF()
ELSE()

ENDIF()

# Inlucde external resources.
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/extern/CMakeLists.txt)


# #TODO resolve and add options and etc.
# # Add platform specific information.
# IF(Android)
# 	MESSAGE(STATUS "Hello from Android build!")
# 	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/platforms/android/)
# ENDIF()
# IF(${CMAKE_SYSTEM_NAME} MATCHES "Tmp")
# 	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/platforms/windows/)
# ENDIF()
# IF(${CMAKE_SYSTEM_NAME} MATCHES "Tmp")
# 	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/platforms/uwp/)
# ENDIF()
# IF(UNIX)
# 	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/platforms/unix/)
# ENDIF()
# IF(APPLE)
# #TODO add support for mac and apple platforms.
# #	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/platforms/unix/)
# ENDIF()

#TODO Add platform source code
IF(UNIX)

ELSEIF(APPLE)

ELSEIF(CYGWIN)

ENDIF()

# Build frag-core.
IF(BUILD_WITH_CORE)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/frag-core)
ENDIF()

# Rendering interfaces.
IF(BUILD_EXTERNAL_RENDERER)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/frag-core/src/renderer/opengl/)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/frag-core/src/renderer/directx/)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/frag-core/src/renderer/vulkan/)
	#TODO changed to a compute base class and module extension.
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/frag-core/src/renderer/opencl/)
ENDIF()

# Build FragCore Audio modules
IF(BUILD_EXTERNAL_AUDIO)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/frag-core/src/audio/openal/)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/frag-core/src/audio/fmod/)
ENDIF()

# Build FragCore Physic modules
IF(BUILD_EXTERNAL_PHYSIC)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/frag-core/src/physic/bullet/)
	#ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/frag-core/src/physic/physx/)
ENDIF()

# Testing and assertion and other.
#TODO attempt to remove so it works on all platforms.
IF(NOT ANDROID)
	IF(BUILD_WITH_TEST)
		ENABLE_TESTING()
		ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/test/)
	ENDIF(BUILD_WITH_TEST)
ENDIF()

# Generate sample executables.
IF(BUILD_WITH_SAMPLES)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/test/examples)
ENDIF()

# Generate documentations.
IF(BUILD_WITH_DOCS)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/docs/)
ENDIF(BUILD_WITH_DOCS)

# Only UNIX systems.
IF( UNIX )
	# Use GNUInstallDirs for Unix predefined directories
	INCLUDE(GNUInstallDirs)

	SET( FRAG_LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
	SET( FRAG_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
	SET( FRAG_BIN_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})

	# 
	Find_PACKAGE(UnixCommands REQUIRED)

	FIND_PROGRAM(GLSLLANGVALIDATOR glslangValidator)
	FIND_PROGRAM(SPIRVDIS spirv-dis)
	FIND_PROGRAM(SPIRVAS spirv-as)
	FIND_PROGRAM(SPIRVREMAP spirv-remap)
	
	IF(GLSLLANGVALIDATOR)
		MESSAGE(STATUS "glslangValidator found: ${GLSLLANGVALIDATOR}")
		#TODO add for all shaders.
		ADD_CUSTOM_TARGET(samples-spirv
				COMMENT "Creating spirv shader from the display shader.")
		#ADD_DEPENDENCIES(fragview samples-spirv)
		FOREACH(glslFile ${SAMPLE_GLSL_FILES})
			MESSAGE(STATUS ${glslFile} )
			ADD_CUSTOM_COMMAND(TARGET samples-spirv PRE_BUILD
					COMMAND glslangValidator --entry-point main --auto-map-bindings --auto-map-locations --uniform-base 0
					-u:time:10 -G450 ${glslFile} -o ${CMAKE_CURRENT_SOURCE_DIR}/samples/spirv/)
		ENDFOREACH()

		# Create default shader
		ADD_CUSTOM_TARGET(display-spirv
				COMMENT "Creating spirv shader from the display shader.")
		#ADD_DEPENDENCIES(fragview display-spirv)
		FOREACH(glslFile ${SAMPLE_SHADER_GLSL_FILES})
			ADD_CUSTOM_COMMAND(TARGET display-spirv PRE_BUILD
					COMMAND glslangValidator --entry-point main --auto-map-bindings --auto-map-locations --uniform-base 0 -G -d
					-u:time:10 ${glslFile} -o ${CMAKE_CURRENT_SOURCE_DIR}/)
		ENDFOREACH()

	ELSE()
		MESSAGE(WARNING "Could not find the glslangValidator program for creating spirv shaders.")
	ENDIF()

	FIND_PROGRAM(ZIP zip)

	IF(ZIP)
		MESSAGE(STATUS "zip found: ${ZIP}")

	ENDIF()
		# Install main icon image.
		ADD_DEFINITIONS(-DICON_LOCATION="/usr/share/fragview/")
		IF(BUILD_WITH_INSTALL)
		INSTALL( FILES ${CMAKE_CURRENT_SOURCE_DIR}/fragview.png DESTINATION /usr/share/fragview/ )
	ENDIF()

	# Create distribution tarball.
	SET( TARGETDIR "${PROJECT_NAME}-${VERSION}")
	ADD_CUSTOM_TARGET(	distribution
			COMMENT "Creating distrubtion file."
			COMMAND mkdir -p ${TARGETDIR}
			COMMAND ${CP} -r   ${CMAKE_CURRENT_SOURCE_DIR}/src
			${CMAKE_CURRENT_SOURCE_DIR}/frag-core

			${CMAKE_CURRENT_SOURCE_DIR}/utils
			${CMAKE_CURRENT_SOURCE_DIR}/test
			${CMAKE_CURRENT_SOURCE_DIR}/images
			${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
			${CMAKE_CURRENT_SOURCE_DIR}/.gitignore
			${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
			${TARGETDIR}
			COMMAND ${TAR} cf - ${TARGETDIR} | ${GZIP} -c > ${TARGETDIR}.tar.gz
			COMMAND ${RM} -r ${TARGETDIR} )

	# Check if desktop icon tools exists.
	FIND_PROGRAM(INSTALL_DESKTOP install-desktop)
	FIND_PROGRAM(INSTALL_DESKTOP_FILE_VALIDATE desktop-file-validate)
	FIND_PROGRAM(DESKTOP_FILE_INSTALL desktop-file-install)
	FIND_PROGRAM(UPDATE_DESKTOP_DATABASE update-desktop-database)

	# Check that the program exists.
	IF(INSTALL_DESKTOP AND INSTALL_DESKTOP_FILE_VALIDATE AND DESKTOP_FILE_INSTALL AND UPDATE_DESKTOP_DATABASE)
		MESSAGE(STATUS "install-desktop: ${INSTALL_DESKTOP}")
		MESSAGE(STATUS "desktop-file-validate: ${INSTALL_DESKTOP_FILE_VALIDATE}")
		MESSAGE(STATUS "desktop-file-install: ${DESKTOP_FILE_INSTALL}")
		MESSAGE(STATUS "update-desktop-database: ${UPDATE_DESKTOP_DATABASE}")

		# Install icons. TODO relocate 
		ADD_CUSTOM_TARGET(install-desktop
				COMMENT "Validating and installing desktop entry."
				COMMAND desktop-file-validate ${CMAKE_CURRENT_SOURCE_DIR}/fragview.desktop
				COMMAND desktop-file-install --rebuild-mime-info-cache ${CMAKE_CURRENT_SOURCE_DIR}/fragview.desktop
				COMMAND update-desktop-database
				DEPENDS INSTALL_DESKTOP INSTALL_DESKTOP_FILE_VALIDATE DESKTOP_FILE_INSTALL UPDATE_DESKTOP_DATABASE)
	ELSE()
		MESSAGE(WARNING "Could not find the program tools for installing the icon and its images.")
	ENDIF()

ELSEIF(WIN32)

ELSE()
	# Cache these to allow the user to override them on non-Unix platforms
	SET( FRAG_LIB_INSTALL_DIR "lib" CACHE STRING
	"Path the built library files are installed to." )
	SET( FRAG_INCLUDE_INSTALL_DIR "include" CACHE STRING
	"Path the header files are installed to." )
	SET( FRAG_BIN_INSTALL_DIR "bin" CACHE STRING
	"Path the tool executables are installed to." )

	SET(CMAKE_INSTALL_FULL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/${FRAG_INCLUDE_INSTALL_DIR})
	SET(CMAKE_INSTALL_FULL_LIBDIR ${CMAKE_INSTALL_PREFIX}/${FRAG_LIB_INSTALL_DIR})
	SET(CMAKE_INSTALL_FULL_BINDIR ${CMAKE_INSTALL_PREFIX}/${FRAG_BIN_INSTALL_DIR})
ENDIF()


FIND_PACKAGE(PythonInterp REQUIRED)
IF(PYTHONINTERP_FOUND)
	#TODO relocate to the fragengine and editor.
	#TODO add as cmake tool
	ADD_CUSTOM_TARGET( compile-shader
			COMMENT "Creating header and source file for display shaders."
			COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/utils/glsl2CString.py ${CMAKE_CURRENT_SOURCE_DIR}/shaders/glsl/
			DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/utils/glsl2CString.py)
ENDIF()

IF(WIN32)
	IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
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

	IF(MSVC12 OR MSVC14 OR MSVC15 )

	ENDIF(MSVC12 OR MSVC14 OR MSVC15 )
ENDIF (WIN32)


# Create install package.
IF ( BUILD_WITH_INSTALL )
	IF(CMAKE_CPACK_COMMAND AND UNIX AND BUILD_OPT_BUILD_PACKAGES)
		# Packing information
		SET(CPACK_PACKAGE_NAME                    "frag${FRAG_VERSION_MAJOR}.${VERSION_MINOR}")
		SET(CPACK_PACKAGE_CONTACT "" CACHE STRING "Package maintainer and PGP signer.")
		SET(CPACK_PACKAGE_VENDOR                  "https://github.com/voldien/fragcore")
		SET(CPACK_PACKAGE_DISPLAY_NAME            "Frag ${VERSION}")
		SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY     " -  ${VERSION}")
		SET(CPACK_PACKAGE_VERSION                 "${VERSION}.${PACKAGE_VERSION}" )
		SET(CPACK_PACKAGE_VERSION_MAJOR           "${VERSION_MAJOR}")
		SET(CPACK_PACKAGE_VERSION_MINOR           "${VERSION_MINOR}")
		SET(CPACK_PACKAGE_VERSION_PATCH           "${VERSION_PATCH}")
		SET(CPACK_PACKAGE_INSTALL_DIRECTORY       "frag${VERSION_MAJOR}.${VERSION_MINOR}")
		SET(CPACK_RESOURCE_FILE_LICENSE           "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

		STRING(TOUPPER ${LIBFRAGCORE_COMPONENT}     "LIBFRAGCORE_COMPONENT_UPPER")
		STRING(TOUPPER ${LIBFRAGCORE-DEV_COMPONENT} "LIBFRAGCORE-DEV_COMPONENT_UPPER")

		#
		SET(CPACK_COMPONENT_FRAGCORE-BIN_DISPLAY_NAME                       "tools")
		SET(CPACK_COMPONENT_FRAGCORE-BIN_DEPENDS                            "${LIBFRAGCORE_COMPONENT}" )
		SET(CPACK_COMPOENTT_${LIBFRAGENGINE_COMPONENT_UPPER}_DISPLAY_NAME "")
		SET(CPACK_COMPOENTT_${LIBFRAGENGINE-DEV_COMPONENT_UPPER}_DISPLAY_NAME "")
		SET(CPACK_COMPONENT_${LIBFRAGENGINE-DEV_COMPONENT_UPPER}_DEPENDS $    "{LIBFRAGCORE_COMPONENT}" )
		SET(CPACK_COMPOENTT_${LIBFRAGENGINE-DEV_COMPONENT_UPPER}_DISPLAY_NAME "")
		SET(CPACK_COMPONENT_${LIBFRAGCORE_COMPONENT_UPPER}_DISPLAY_NAME     "libraries")
		SET(CPACK_COMPONENT_${LIBFRAGCORE-DEV_COMPONENT_UPPER}_DISPLAY_NAME "common headers and installs")
		SET(CPACK_COMPONENT_${LIBFRAGCORE-DEV_COMPONENT_UPPER}_DEPENDS $    "{LIBFRAGCORE_COMPONENT}" )
		SET(CPACK_COMPONENT_LIBFRAGCORE-DEV_DISPLAY_NAME                       "${CPACK_COMPONENT_${LIBFRAGCORE-DEV_COMPONENT}_DISPLAY_NAME}" )
		SET(CPACK_COMPONENT_LIBFRAGCORE-DEV_DEPENDS                            "${LIBFRAGCORE-DEV_COMPONENT}" )
		SET(CPACK_DEBIAN_BUILD_DEPENDS debhelper cmake zlib1g-dev pkg-config)

		# debian
		SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
		SET(CPACK_DEBIAN_CMAKE_OPTIONS    "")
		SET(CPACK_DEBIAN_PACKAGE_SECTION  "libs" )
		SET(CPACK_DEBIAN_PACKAGE_DEPENDS  "${CPACK_COMPONENTS_ALL}")
		SET(CPACK_DEBIAN_PACKAGE_SUGGESTS)
		SET(cPACK_DEBIAN_PACKAGE_NAME     "fragengine")
		SET(CPACK_DEBIAN_PACKAGE_REMOVE_SOURCE_FILES build doc obj samples packaging)
		SET(CPACK_DEBIAN_PACKAGE_SOURCE_COPY svn export --force)
		SET(CPACK_DEBIAN_CHANGELOG)
		execute_process(COMMAND lsb_release -is
			OUTPUT_VARIABLE _lsb_distribution OUTPUT_STRIP_TRAILING_WHITESPACE
			RESULT_VARIABLE _lsb_release_failed)
		SET(CPACK_DEBIAN_DISTRIBUTION_NAME ${_lsb_distribution} CACHE STRING "Name of the distrubiton")
		STRING(TOLOWER ${CPACK_DEBIAN_DISTRIBUTION_NAME} CPACK_DEBIAN_DISTRIBUTION_NAME)
		IF( ${CPACK_DEBIAN_DISTRIBUTION_NAME} STREQUAL "ubuntu" )
			SET(CPACK_DEBIAN_DISTRIBUTION_RELEASES lucid maverick natty oneiric precise CACHE STRING "Release code-names of the distrubiton release")
		ENDIF()
		SET(DPUT_HOST "" CACHE STRING "PPA repository to upload the debian sources")
		INCLUDE(CPack)
		INCLUDE(DebSourcePPA)
	ENDIF()
ENDIF()

# Final verbose message status.
MESSAGE(STATUS "")
MESSAGE(STATUS "These are the configured paths:")
MESSAGE(STATUS "  PREFIX: ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "  LIB_DIR: ${LIB_DIR}")
MESSAGE(STATUS "  RENDERING_DIR: ${PLUGIN_DIR}")
MESSAGE(STATUS "  BIN_DIR: ${BIN_DIR}")
MESSAGE(STATUS "  DATA_DIR: ${DATA_DIR}")
MESSAGE(STATUS "  MAN_DIR: ${MAN_DIR}")
MESSAGE(STATUS "  DOC_DIR: ${DOC_DIR}")
MESSAGE(STATUS "  ICON_DIR: ${ICON_DIR}")
MESSAGE(STATUS "  SVG_DIR: ${SVG_DIR}")
MESSAGE(STATUS "  MENU_DIR: ${MENU_DIR}")
MESSAGE(STATUS "")
MESSAGE(STATUS "Options:")
MESSAGE(STATUS "  LAYOUT: ${LAYOUT}")
MESSAGE(STATUS "")
MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "Target bitness: ${CMAKE_SIZEOF_VOID_P}*8")
MESSAGE(STATUS "")
