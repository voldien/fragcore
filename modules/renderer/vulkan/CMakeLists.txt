# Add support for compiling with debug tracer enable in the code.
IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
	#TODO resolve with another cmake function.
	ADD_DEFINITIONS(-DDEBUG_TRACKER)
ENDIF()

FIND_PACKAGE(Vulkan QUIET)
IF(Vulkan_FOUND)
    MESSAGE(STATUS "Vulkan: ${Vulkan_LIBRARY}")
ENDIF()

FILE(GLOB VULKAN_RENDER_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB VULKAN_RENDER_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)


ADD_LIBRARY(fragcore-rvk-obj OBJECT ${VULKAN_RENDER_SOURCE_FILES} ${VULKAN_RENDER_HEADER_FILES})
TARGET_LINK_LIBRARIES(fragcore-rvk-obj PUBLIC ${Vulkan_LIBRARIES} fragcore-window-sdl fragcore fragcore-render-interface PRIVATE vkcommon-core) #
TARGET_COMPILE_DEFINITIONS(fragcore-rvk-obj PRIVATE FRAG_CORE_INTERNAL_IMP=1)

#
TARGET_INCLUDE_DIRECTORIES(fragcore-rvk-obj PRIVATE ${Vulkan_INCLUDE_DIRS} PUBLIC
											$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
											$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
ADD_DEPENDENCIES(fragcore-rvk-obj fragcore vkcommon-core fragcore-render-interface fragcore-window-sdl)


# Library target.
ADD_LIBRARY(fragcore-rvk $<TARGET_OBJECTS:fragcore-rvk-obj>)
TARGET_INCLUDE_DIRECTORIES(fragcore-rvk PRIVATE ${Vulkan_INCLUDE_DIRS} PUBLIC
											$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
											$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

ADD_LIBRARY(fragcore-module-render-vulkan MODULE $<TARGET_OBJECTS:fragcore-rvk-obj>  )
TARGET_LINK_LIBRARIES(fragcore-module-render-vulkan INTERFACE fragcore-rvk )

#
IF(BUILD_WITH_INSTALL)
    INSTALL(TARGETS fragcore-rvk DESTINATION lib COMPONENT ${LIBFRAGCORE_COMPONENT})
ENDIF()

IF(CMAKE_TESTING_ENABLED)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/test)
ENDIF()

