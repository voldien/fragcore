SET(OpenGL_GL_PREFERENCE "GLVND")

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
INCLUDE(bullet3)

# Source files.
FILE(GLOB SOURCE_PHYSIC_BULLET_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB HEADER_PHYSIC_BULLET_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

ADD_LIBRARY(fragcore-physic-bullet ${SOURCE_PHYSIC_BULLET_FILES} ${HEADER_PHYSIC_BULLET_FILES} )
TARGET_LINK_LIBRARIES(fragcore-physic-bullet fragcore fragcore-physic-interface BulletCollision BulletDynamics BulletSoftBody)

TARGET_INCLUDE_DIRECTORIES(fragcore-physic-bullet PUBLIC ${libbullet3_source_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR} )

ADD_DEPENDENCIES(fragcore-physic-bullet fragcore fragcore-physic-interface BulletCollision BulletDynamics BulletSoftBody)

IF (BUILD_SHARED_LIBS AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	SET_PROPERTY(TARGET fragcore-physic-bullet PROPERTY POSITION_INDEPENDENT_CODE ON)
ENDIF()

INSTALL(TARGETS fragcore-physic-bullet fragcore-physic-interface DESTINATION lib)
