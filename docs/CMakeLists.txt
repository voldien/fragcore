FIND_PACKAGE( Doxygen )

IF(DOXYGEN_FOUND)
    MESSAGE(STATUS "Doxygen found: ${DOXYGEN_EXECUTABLE} -- ${DOXYGEN_VERSION}")

	# Set Doxygen input and output files.
	SET(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/include)
	SET(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
	SET(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
	SET(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
	SET(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

	# Generate DoxyFile from the input file.
	CONFIGURE_FILE(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

	# Create Output directory.
	FILE(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})


	# Command for generating doc from Doxygen config file.
	ADD_CUSTOM_COMMAND(OUTPUT ${DOXYGEN_INDEX_FILE}
					COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
					MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
					COMMENT "Generating TaskScheduler Doxygen documentation"
					VERBATIM)

	# Create CMake Target for generating doc.
	ADD_CUSTOM_TARGET(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

ENDIF()
