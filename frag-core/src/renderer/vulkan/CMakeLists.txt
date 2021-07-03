# Add support for compiling with debug tracer enable in the code.
IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
	#TODO resolve with another cmake function.
	ADD_DEFINITIONS(-DDEBUG_TRACKER)
ENDIF()

FIND_PACKAGE(Vulkan)
IF(Vulkan_FOUND)
    MESSAGE(STATUS "Vulkan: ${Vulkan_LIBRARY}")
ENDIF()

FILE(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Get the include directories for the target.
get_target_property(Fragcore_INCLUDE_DIRS fragcore INCLUDE_DIRECTORIES)

# Library target.
ADD_LIBRARY(fragcore-rvk ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(fragcore-rvk ${Vulkan_LIBRARIES} fragcore) #
TARGET_COMPILE_DEFINITIONS(fragcore-rvk PRIVATE FRAG_CORE_INTERNAL_IMP=1)

#
TARGET_INCLUDE_DIRECTORIES(fragcore-rvk PRIVATE ${Vulkan_INCLUDE_DIRS} ${Fragcore_INCLUDE_DIRS} vkcommon)
ADD_DEPENDENCIES(fragcore-rvk fragcore vkcommon)

#
IF(BUILD_WITH_INSTALL)
    INSTALL(TARGETS fragcore-rvk DESTINATION lib COMPONENT ${LIBFRAGCORE_COMPONENT})
ENDIF()
