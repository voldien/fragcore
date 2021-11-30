
FIND_PACKAGE(GTest REQUIRED)
IF(GTEST_FOUND)
	MESSAGE(STATUS "Gtest found: ${GTEST_BOTH_LIBRARIES}")
ELSE()
	MESSAGE(ERROR "Gtest is required in order to compile and run the tests")
ENDIF()

# Source files.
FILE(GLOB SOURCE_FILES 	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/src/dataStructure/*.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/src/math/*.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp
						${CMAKE_CURRENT_SOURCE_DIR}/src/network/*.cpp)
FILE(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

ADD_EXECUTABLE(fragcore-assert ${SOURCE_FILES} ${HEADER_FILES})
TARGET_LINK_LIBRARIES(fragcore-assert ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} fragcore pthread )
TARGET_INCLUDE_DIRECTORIES(fragcore-assert PRIVATE ${GTEST_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include/ )
ADD_DEPENDENCIES(fragcore-assert fragcore)

INSTALL(TARGETS fragcore-assert DESTINATION bin)
ADD_TEST(NAME assert-functionality
		COMMAND fragcore-assert
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
