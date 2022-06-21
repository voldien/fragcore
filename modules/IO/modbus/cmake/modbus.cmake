#
# Builds the modbus library
#
# Outputs the following target:
#   modbus
#

INCLUDE(FetchContent)

FetchContent_Declare(libmodbus_source
	GIT_REPOSITORY https://github.com/stephane/libmodbus.git
	GIT_TAG v3.1.7
) # or whatever tag you want

FetchContent_GetProperties(libmodbus_source)

IF(NOT libmodbus_source)
	FetchContent_Populate(libmodbus_source)

  include(ExternalProject)
	SET(MODBUS_DIR ${libmodbus_source_SOURCE_DIR})
	SET(MODBUS_BIN ${libmodbus_source_BINARY_DIR})
  set(MODBUS_STATIC_LIB ${MODBUS_BIN}/lib/libmodbus.a)
  set(MODBUS_INCLUDES ${MODBUS_BIN}/include)
  
  file(MAKE_DIRECTORY ${MODBUS_INCLUDES})
  
  ExternalProject_Add(
    libmodbus
    PREFIX ${MODBUS_BIN}
    SOURCE_DIR ${MODBUS_DIR}
    DOWNLOAD_COMMAND cd ${MODBUS_DIR} && git clean -dfX &&  ${MODBUS_DIR}/autogen.sh
    CONFIGURE_COMMAND ${MODBUS_DIR}/configure --srcdir=${MODBUS_DIR} --prefix=${MODBUS_BIN} --enable-static=yes --disable-shared
    BUILD_COMMAND make -j ${CMAKE_BUILD_PARALLEL_LEVEL}
    INSTALL_COMMAND make install
    BUILD_BYPRODUCTS ${MODBUS_STATIC_LIB}
  )
  
  add_library(modbus STATIC IMPORTED GLOBAL)
  
  add_dependencies(modbus libmodbus)
  
  set_target_properties(modbus PROPERTIES IMPORTED_LOCATION ${MODBUS_STATIC_LIB})
  set_target_properties(modbus PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${MODBUS_INCLUDES})
  
ELSE()
  MESSAGE( WARNING "Could not find modbus source code")
ENDIF()
