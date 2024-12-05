
FIND_PACKAGE(GTest REQUIRED)
IF(GTEST_FOUND)
	MESSAGE(STATUS "Gtest found: ${GTEST_BOTH_LIBRARIES}")
ELSE()
	MESSAGE(ERROR "Gtest is required in order to compile and run the tests")
ENDIF()

# Source files.
FILE(GLOB SOURCE_FILES 	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

ADD_EXECUTABLE(fragcore-test-compression ${SOURCE_FILES} )
TARGET_LINK_LIBRARIES(fragcore-test-compression ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} fragcore-compression )
TARGET_INCLUDE_DIRECTORIES(fragcore-test-compression PRIVATE ${GTEST_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include/ )
ADD_DEPENDENCIES(fragcore-test-compression fragcore)

INSTALL(TARGETS fragcore-test-compression DESTINATION bin)
ADD_TEST(NAME assert-compression
		COMMAND fragcore-test-compression
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
