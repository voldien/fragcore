
# GCC compiler flag options.
IF ((CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX) AND NOT CMAKE_COMPILER_IS_MINGW)

ENDIF ()

FIND_PACkAGE(Bullet)
SET(BULLET_LIB "-lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody")
IF(BULLET_FOUND)

ENDIF()

# Source files.
FILE(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Get the include directories for the target.
get_target_property(Fragcore_INCLUDE_DIRS fragcore INCLUDE_DIRECTORIES)

ADD_LIBRARY(fragcore-pbu ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(fragcore-pbu ${BULLET_LIB} fragcore ${BULLET_LIBRARIES} )

TARGET_INCLUDE_DIRECTORIES(fragcore-pbu PRIVATE ${Vulkan_INCLUDE_DIRS} ${Fragcore_INCLUDE_DIRS})

ADD_DEPENDENCIES(fragcore-pbu fragcore)
TARGET_INCLUDE_DIRECTORIES( fragcore-pbu PUBLIC "${BULLET_INCLUDE_DIRS}" )
INSTALL(TARGETS fragcore-pbu DESTINATION lib)
