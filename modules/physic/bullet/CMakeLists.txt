
FIND_PACkAGE(Bullet QUIET)
SET(BULLET_LIB "-lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody")
IF(BULLET_FOUND)
    MESSAGE(STATUS "Bullet found: ${BULLET_LIBRARY}.")
ENDIF()

# Source files.
FILE(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

ADD_LIBRARY(fragcore-pbu ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(fragcore-pbu ${BULLET_LIB} fragcore ${BULLET_LIBRARIES} )

TARGET_INCLUDE_DIRECTORIES( fragcore-pbu PRIVATE "${BULLET_INCLUDE_DIRS}" )

ADD_DEPENDENCIES(fragcore-pbu fragcore)


INSTALL(TARGETS fragcore-pbu DESTINATION lib)
