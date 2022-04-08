#
# Builds the serial port library
#
# Outputs the following target:
#   serialport
#
INCLUDE(FetchContent)

FetchContent_Declare(libserialport_source
	GIT_REPOSITORY https://github.com/sigrokproject/libserialport.git
	GIT_TAG master
) # or whatever tag you want

FetchContent_GetProperties(libserialport_source)

IF(NOT libserialport_source)
	FetchContent_Populate(libserialport_source)


	INCLUDE(ExternalProject)
	SET(SERIALPORT_DIR ${libserialport_source_SOURCE_DIR})
	SET(SERIALPORT_BIN ${libserialport_source_BINARY_DIR})
	SET(SERIALPORT_LIB ${SERIALPORT_BIN}/lib/libserialport.a)
	SET(SERIALPORT_INCLUDE_DIRS ${SERIALPORT_BIN}/include)

	FILE(MAKE_DIRECTORY ${SERIALPORT_INCLUDE_DIRS})

	ExternalProject_Add(
		libserialport
		#DEPENDS libserialport_source
		PREFIX ${SERIALPORT_BIN}
		SOURCE_DIR ${SERIALPORT_DIR}
		DOWNLOAD_COMMAND cd ${SERIALPORT_DIR} && git clean -dfX && ${SERIALPORT_DIR}/autogen.sh
		CONFIGURE_COMMAND ${SERIALPORT_DIR}/configure --srcdir=${SERIALPORT_DIR} --prefix=${SERIALPORT_BIN} --enable-static=yes --disable-shared  CFLAGS=-fPIC CXXFLAGS=-fPIC
		BUILD_COMMAND make
		BUILD_BYPRODUCTS ${SERIALPORT_LIB}
	)

	add_library(serialport STATIC IMPORTED GLOBAL)

	add_dependencies(serialport libserialport)

	set_target_properties(serialport PROPERTIES IMPORTED_LOCATION ${SERIALPORT_LIB})
	set_target_properties(serialport PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SERIALPORT_INCLUDE_DIRS})
ELSE()
	MESSAGE( WARNING "Could not find Serialport source code");
ENDIF()	