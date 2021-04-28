
OPTION(BUILTIN_NANOSVG "" ON)
OPTION(BUILTIN_EIGEN "" ON)
OPTION(BUILTIN_OPEN_SIMPLEX_NOISE "" ON)
OPTION(BUILTIN_XATLAS "" ON)
OPTION(BUILTIN_PCG_C_BASIC "" ON)
OPTION(BUILTIN_RECASTNAVIGATION "" ON)
OPTION(BUILTIN_POLYPARITION "" ON)
OPTION(BUILTIN_KISS_FFT "" ON)
OPTION(BUILTIN_MISC "" ON)

SET(THIRDPARTY_INCLUDE_DIR "")
SET(THIRDPARTY_SOURCE "")

IF(BUILTIN_NANOSVG)
	SET(NANOSVG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/nanosvg)

	SET(THIRDPARTY_INCLUDE_DIR "${THIRDPARTY_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/nanosvg")
	FILE(GLOB NANOSVG_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/nanosvg/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/extern/nanosvg/*.cc)

	SET(THIRDPARTY_SOURCE "${THIRDPARTY_SOURCE};${NANOSVG_SOURCE}")
ENDIF()

IF( BUILTIN_EIGEN)
	SET(EIGEN_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/eigen/Eigen)

ENDIF()

IF(BUILTIN_OPEN_SIMPLEX_NOISE )
	SET(OPEN_SIMPLEX_NOISE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/open-simplex-noise-in-c)

	SET(THIRDPARTY_INCLUDE_DIR  "${THIRDPARTY_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/open-simplex-noise-in-c")
	FILE(GLOB OPEN_SIMPLEX_NOISE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/open-simplex-noise-in-c/*.c )
	SET(THIRDPARTY_SOURCE "${THIRDPARTY_SOURCE};${OPEN_SIMPLEX_NOISE_SOURCE}")
ENDIF()
IF(BUILTIN_XATLAS )
	SET(XATLAS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/xatlas)

	SET(THIRDPARTY_INCLUDE_DIR  "${THIRDPARTY_INCLUDE_DIR}  ${CMAKE_CURRENT_SOURCE_DIR}/xatlas")
	FILE(GLOB XATLAS_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/xatlas/*.h
							${CMAKE_CURRENT_SOURCE_DIR}/extern/xatlas/*.cpp )

	SET(THIRDPARTY_SOURCE "${THIRDPARTY_SOURCE};${XATLAS_SOURCE}")
ENDIF()

IF(BUILTIN_PCG_C_BASIC)
	SET(PCG_C_BASIC_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/pcg-c-basic)

	SET(THIRDPARTY_INCLUDE_DIR  "${THIRDPARTY_INCLUDE_DIR}  ${CMAKE_CURRENT_SOURCE_DIR}/extern/pcg-c-basic")
	FILE(GLOB PCG_C_BASIC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/pcg-c-basic/*.h
							${CMAKE_CURRENT_SOURCE_DIR}/extern/pcg-c-basic/*.c )

	SET(THIRDPARTY_SOURCE "${THIRDPARTY_SOURCE};${PCG_C_BASIC_SOURCE}")
ENDIF()

IF(BUILTIN_RECASTNAVIGATION)
	SET(RECASTNAVIGATION_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/recastnavigation/Include)

	SET(THIRDPARTY_INCLUDE_DIR  "${THIRDPARTY_INCLUDE_DIR}  ${CMAKE_CURRENT_SOURCE_DIR}/recastnavigation")
	FILE(GLOB RECASTNAVIGATION_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/recastnavigation/Include/*.h
										${CMAKE_CURRENT_SOURCE_DIR}/extern/recastnavigation/Source/*.cpp )

	SET(THIRDPARTY_SOURCE "${THIRDPARTY_SOURCE};${RECASTNAVIGATION_SOURCE}")
ENDIF()


IF(BUILTIN_POLYPARITION)
	SET(POLYPARITION_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/polypartition)


	SET(THIRDPARTY_INCLUDE_DIR  "${THIRDPARTY_INCLUDE_DIR}  ${POLYPARITION_INCLUDE_DIRS}")
	FILE(GLOB POLYPARITION_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/polypartition/*.h
										${CMAKE_CURRENT_SOURCE_DIR}/extern/polypartition/*.cpp)

	SET(THIRDPARTY_SOURCE "${THIRDPARTY_SOURCE};${POLYPARITION_SOURCE}")
ENDIF()


IF(BUILTIN_KISS_FFT)
	#https://github.com/mborgerding/kissfft.git
	SET(KISS_FFT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/kissfft)


	SET(THIRDPARTY_INCLUDE_DIR  "${THIRDPARTY_INCLUDE_DIR}  ${KISS_FFT_INCLUDE_DIRS}")
	FILE(GLOB KISS_FFT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/kissfft/*.h
										${CMAKE_CURRENT_SOURCE_DIR}/extern/kissfft/*.c)

	SET(THIRDPARTY_SOURCE "${THIRDPARTY_SOURCE};${KISS_FFT_SOURCE}")
ENDIF()

IF(BUILTIN_MISC)
	SET(MISC_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/extern/misc)


	SET(THIRDPARTY_INCLUDE_DIR  "${THIRDPARTY_INCLUDE_DIR}  ${MISC_INCLUDE_DIRS}")
	FILE(GLOB MISC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/extern/misc/*.h
										${CMAKE_CURRENT_SOURCE_DIR}/extern/misc/*.c
										${CMAKE_CURRENT_SOURCE_DIR}/extern/misc/*.cpp)


	SET(THIRDPARTY_SOURCE "${THIRDPARTY_SOURCE};${MISC_SOURCE}")
ENDIF()



# SET(ExternalProjectCMakeArgs
#     -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/external
#     -DCMAKE_Fortran_COMPILER=${CMAKE_Fortran_COMPILER}
#     -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#     -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#     -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#     )

IF(BUILD_WITH_EXTERNAL_PROJECT)

	# Git must be avaiable on the system for using the external projects.
	IF(GIT_FOUND)
		INCLUDE(ExternalProject)
		SET_PROPERTY(DIRECTORY PROPERTY EP_STEP_TARGETS build)

		SET(TASKSCH_ROOT ${CMAKE_CURRENT_BINARY_DIR}/deps/task-sch)
		SET(TASKSCH_LIB_DIR ${TASKSCH_ROOT}/lib)
		SET(TASKSCH_INCLUDE_DIR ${TASKSCH_ROOT}/include)

		ExternalProject_Add(tasksch-external
				URL ${CMAKE_CURRENT_SOURCE_DIR}/extern/task-scheduler
				SOURCE_DIR ${TASKSCH_ROOT}/src/tasksch
				BINARY_DIR ${TASKSCH_ROOT}/src/tasksch-build
				INSTALL_DIR ${TASKSCH_ROOT}
				CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_WITH_TEST=OFF
				STEP_TARGETS build
				COMMENT "Building task-schedular."
				LOG_DOWNLOAD ON)

		# Include tasksch headers.
		ADD_LIBRARY(taskSch SHARED IMPORTED)
		ADD_DEPENDENCIES(taskSch tasksch-external)
		SET_TARGET_PROPERTIES(taskSch PROPERTIES
				IMPORTED_LOCATION ${TASKSCH_LIB_DIR}/libtaskSch.so
				INTERFACE_LINK_LIBRARIES ${TASKSCH_LIB_DIR}/libtaskSch.so
				INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/extern/task-scheduler/include
				INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/extern/task-scheduler/include)

		# Download the hpm library.
		SET(HPM_ROOT ${CMAKE_CURRENT_BINARY_DIR}/deps/hpm)
		SET(HPM_LIB_DIR ${HPM_ROOT}/lib)
		SET(HPM_INCLUDE_DIR ${HPM_ROOT}/include)
		ExternalProject_Add(hpm-external
				URL ${CMAKE_CURRENT_SOURCE_DIR}/extern/hpmcpp/extern/hpm
				SOURCE_DIR ${HPM_ROOT}/src/hpm
				BINARY_DIR ${HPM_ROOT}/src/hpm-build
				INSTALL_DIR ${HPM_ROOT}
				CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_TEST=OFF
				STEP_TARGETS build
				COMMENT "Building Hpm."
				LOG_DOWNLOAD ON)
		# Include HPM headers.
		ADD_LIBRARY(hpm SHARED IMPORTED)
		ADD_DEPENDENCIES(hpm hpm-external)
		SET_TARGET_PROPERTIES(hpm PROPERTIES
				IMPORTED_LOCATION ${HPM_LIB_DIR}/libhpm.so
				INTERFACE_LINK_LIBRARIES ${HPM_LIB_DIR}/libhpm.so
				INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/extern/hpmcpp/extern/hpm/include
				INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/extern/hpmcpp/extern/hpm/include)

		# Download the HPMCPP library.
		SET(HPMCPP_ROOT ${CMAKE_CURRENT_BINARY_DIR}/deps/hpmcpp)
		SET(HPMCPP_LIB_DIR ${HPMCPP_ROOT}/lib)
		SET(HPMCPP_INCLUDE_DIR ${HPMCPP_ROOT}/include)
		ExternalProject_Add(hpmcpp-external
				URL ${CMAKE_CURRENT_SOURCE_DIR}/extern/hpmcpp
				SOURCE_DIR ${HPMCPP_ROOT}/src/hpmcpp
				BINARY_DIR ${HPMCPP_ROOT}/src/hpmcpp-build
				INSTALL_DIR ${HPMCPP_ROOT}
				CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DGIT_SUBMODULE=OFF
				STEP_TARGETS hpm-external build
				COMMENT "Building HPM-CPP"
				LOG_DOWNLOAD ON)

		# Include HPMCPP headers.
		ADD_LIBRARY(hpmcpp SHARED IMPORTED)
		ADD_DEPENDENCIES(hpmcpp hpmcpp-external)
		SET_TARGET_PROPERTIES(hpmcpp PROPERTIES
				IMPORTED_LOCATION ${HPMCPP_LIB_DIR}/libhpmcpp.so
				INTERFACE_LINK_LIBRARIES ${HPMCPP_LIB_DIR}/libhpmcpp.so
				INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/extern/hpmcpp/include
				INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/extern/hpmcpp/include)

	# Extract the include directories targets.
	GET_TARGET_PROPERTY(HPMCPP_INCLUDE_DIRS hpmcpp INCLUDE_DIRECTORIES)
	GET_TARGET_PROPERTY(HPM_INCLUDE_DIRS hpm INCLUDE_DIRECTORIES)
	GET_TARGET_PROPERTY(TASKSCH_INCLUDE_DIRS taskSch INCLUDE_DIRECTORIES)

	ELSE()
		MESSAGE(FATAL_ERROR "Require Git for loading external projects.")
	ENDIF()
ENDIF()

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/backward-cpp)

IF(BUILTIN_JEMALLOC)
	#https://github.com/jemalloc/jemalloc.git
ENDIF()

# 	# https://github.com/googlefonts/noto-fonts
# 	# https://github.com/source-foundry/Hack
# 	# https://github.com/google/etc2comp

# 	#https://gitlab.com/libeigen/eigen
# 	# https://github.com/baldurk/renderdoc.git
# 	# https://github.com/ispc/ispc.git
# 	# https://github.com/mborgerding/kissfft.git
# 	# https://github.com/NVIDIAGameWorks/PhysX.git

# 	# https://github.com/jemalloc/jemalloc.git
# 	# https://github.com/turbulenz/NvTriStrip.git
# 	# https://github.com/microsoft/ShaderConductor.git
# 	# https://github.com/AcademySoftwareFoundation/openexr.git
# 	# https://github.com/erikd/libsamplerate.git
# 	# https://bitbucket.org/jthlim/pvrtccompressor
# 	# https://sourceforge.net/projects/libsquish
# 	# https://github.com/syoyo/tinyexr
# 	# https://github.com/tatsuhiro-t/wslay.git

# ELSE()
# 	MESSAGE(WARNING "Require Git for loading external source codes.")
# ENDIF()

# Check that all the depdends variables are found
