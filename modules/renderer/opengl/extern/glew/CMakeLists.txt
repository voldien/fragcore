cmake_minimum_required(VERSION 3.16)
project(glew LANGUAGES C)

if ( NOT DEFINED CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING "Build type" )
endif ()

include(GNUInstallDirs)

set(CMAKE_DEBUG_POSTFIX d)

option (BUILD_UTILS "utilities" ON)
option (GLEW_REGAL "Regal mode" OFF)
option (GLEW_OSMESA "OSMesa mode" OFF)
if (APPLE)
    option (BUILD_FRAMEWORK "Build Framework bundle for OSX" OFF)
endif ()
option (GLEW_X11 "X11 mode" ON)
option (GLEW_EGL "EGL mode" OFF)
option (BUILD_SHARED_LIBS "install/link shared instead of static libs" ON)

set (GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# get version from config/version
file (STRINGS ${GLEW_DIR}/config/version  _VERSION_MAJOR_STRING REGEX "GLEW_MAJOR[ ]*=[ ]*[0-9]+.*")
string (REGEX REPLACE "GLEW_MAJOR[ ]*=[ ]*([0-9]+)" "\\1" CPACK_PACKAGE_VERSION_MAJOR ${_VERSION_MAJOR_STRING})
file (STRINGS ${GLEW_DIR}/config/version  _VERSION_MINOR_STRING REGEX "GLEW_MINOR[ ]*=[ ]*[0-9]+.*")
string (REGEX REPLACE "GLEW_MINOR[ ]*=[ ]*([0-9]+)" "\\1" CPACK_PACKAGE_VERSION_MINOR ${_VERSION_MINOR_STRING})
file (STRINGS ${GLEW_DIR}/config/version  _VERSION_PATCH_STRING REGEX "GLEW_MICRO[ ]*=[ ]*[0-9]+.*")
string (REGEX REPLACE "GLEW_MICRO[ ]*=[ ]*([0-9]+)" "\\1" CPACK_PACKAGE_VERSION_PATCH ${_VERSION_PATCH_STRING})
set (GLEW_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})

if (NOT GLEW_CUSTOM_OUTPUT_DIRS)
  set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

find_package (OpenGL REQUIRED)

# prefer GLVND
if (OPENGL_opengl_LIBRARY)
  set (GLEW_LIBRARIES ${OPENGL_opengl_LIBRARY})
else ()
  set (GLEW_LIBRARIES ${OPENGL_gl_LIBRARY})
endif ()

# X11 required except for Windows and Apple OSX platforms
if (GLEW_X11 AND NOT WIN32 AND NOT APPLE)
  find_package (X11)
  list (APPEND GLEW_LIBRARIES ${OPENGL_glx_LIBRARY} ${X11_LIBRARIES})
endif()

if (WIN32)
  set (GLEW_LIB_NAME glew32)
else ()
  set (GLEW_LIB_NAME GLEW)
  set (DLL_PREFIX lib)
endif ()

add_definitions (-DGLEW_NO_GLU)

#### Regal mode ####

if (GLEW_REGAL)
  if (WIN32)
    set (REGAL_LIB_NAME regal32)
  else ()
    set (REGAL_LIB_NAME Regal)
  endif ()
  add_definitions (-DGLEW_REGAL)
  set (GLEW_LIBRARIES ${REGAL_LIB_NAME})
endif ()

#### OSMesa mode ####

if (GLEW_OSMESA)
  if (WIN32)
    set (OSMESA_LIB_NAME osmesa)
  else ()
    set (OSMESA_LIB_NAME OSMesa)
  endif ()
  add_definitions (-DGLEW_OSMESA)
  list (APPEND GLEW_LIBRARIES ${OSMESA_LIB_NAME})
  set (X11_LIBRARIES)
endif ()

#### EGL ####

if (GLEW_EGL AND UNIX)
  add_definitions (-DGLEW_EGL)
  if (NOT OpenGL_EGL_FOUND)
    message (FATAL_ERROR "EGL library set but not found.")
  endif ()
  list (APPEND GLEW_LIBRARIES ${OPENGL_egl_LIBRARY})
endif ()

#### GLEW ####

include_directories (${GLEW_DIR}/include ${X11_INCLUDE_DIR})

set (GLEW_PUBLIC_HEADERS_FILES
  ${GLEW_DIR}/include/GL/wglew.h
  ${GLEW_DIR}/include/GL/glew.h
  ${GLEW_DIR}/include/GL/glxew.h
  ${GLEW_DIR}/include/GL/eglew.h
)
set (GLEW_SRC_FILES ${GLEW_DIR}/src/glew.c)

if (WIN32)
  list (APPEND GLEW_SRC_FILES ${GLEW_DIR}/build/glew.rc)
endif ()

add_library (glew SHARED ${GLEW_PUBLIC_HEADERS_FILES} ${GLEW_SRC_FILES})
set_target_properties (glew PROPERTIES COMPILE_DEFINITIONS "GLEW_BUILD" OUTPUT_NAME "${GLEW_LIB_NAME}" PREFIX "${DLL_PREFIX}"
                                       VERSION ${GLEW_VERSION}
                                       SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR})
TARGET_INCLUDE_DIRECTORIES (glew PUBLIC ${GLEW_DIR}/include ${X11_INCLUDE_DIR})

add_library (glew_s STATIC ${GLEW_PUBLIC_HEADERS_FILES} ${GLEW_SRC_FILES})
set_target_properties (glew_s PROPERTIES COMPILE_DEFINITIONS "GLEW_STATIC" OUTPUT_NAME "${GLEW_LIB_NAME}" PREFIX lib)

if (MSVC)
  # add options from visual studio project
  target_compile_definitions (glew PRIVATE "GLEW_BUILD;VC_EXTRALEAN")
  target_compile_definitions (glew_s PRIVATE "GLEW_STATIC;VC_EXTRALEAN")
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i?86|x86|x86_32)$")
	  target_link_libraries (glew LINK_PRIVATE -BASE:0x62AA0000)
  endif()
  # kill security checks which are dependent on stdlib
  target_compile_options (glew PRIVATE -GS-)
  target_compile_options (glew_s PRIVATE -GS-)
  # remove stdlib dependency
  target_link_libraries (glew LINK_PRIVATE -nodefaultlib -noentry)
  target_link_libraries (glew LINK_PRIVATE libvcruntime.lib)
  target_link_libraries (glew LINK_PRIVATE msvcrt.lib )
  string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
elseif (WIN32 AND ((CMAKE_C_COMPILER_ID MATCHES "GNU") OR (CMAKE_C_COMPILER_ID MATCHES "Clang")))
  # remove stdlib dependency on windows with GCC and Clang (for similar reasons
  # as to MSVC - to allow it to be used with any Windows compiler)
  target_compile_options (glew PRIVATE -fno-builtin -fno-stack-protector)
  target_compile_options (glew_s PRIVATE -fno-builtin -fno-stack-protector)
  target_link_libraries (glew LINK_PRIVATE -nostdlib)
endif ()

if (BUILD_FRAMEWORK)
  set_target_properties(glew PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION ${GLEW_VERSION}
    MACOSX_FRAMEWORK_IDENTIFIER net.sourceforge.glew
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${GLEW_VERSION}
    MACOSX_FRAMEWORK_BUNDLE_VERSION ${GLEW_VERSION}
    XCODE_ATTRIBUTE_INSTALL_PATH "@rpath"
    PUBLIC_HEADER "${GLEW_PUBLIC_HEADERS_FILES}"
    OUTPUT_NAME GLEW
  )
endif()

target_link_libraries (glew LINK_PUBLIC ${GLEW_LIBRARIES})
target_link_libraries (glew_s ${GLEW_LIBRARIES})

target_compile_definitions(glew_s INTERFACE "GLEW_STATIC")
foreach(t glew glew_s)
  target_include_directories(${t} PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
endforeach()

set(targets_to_install "")
if(BUILD_SHARED_LIBS)
  list(APPEND targets_to_install glew)
else ()
  list(APPEND targets_to_install glew_s)
endif()

install ( TARGETS ${targets_to_install}
          EXPORT glew-targets
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
          FRAMEWORK DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT Runtime
)

set (prefix ${CMAKE_INSTALL_PREFIX})
set (exec_prefix ${CMAKE_INSTALL_PREFIX})
set (libdir ${CMAKE_INSTALL_FULL_LIBDIR})
set (includedir ${CMAKE_INSTALL_FULL_INCLUDEDIR})
set (version ${GLEW_VERSION})
set (libname ${GLEW_LIB_NAME})
set (cflags)
set (requireslib glu)

#  Mac OSX has no glu.pc unless optional X11/GLX is installed
if (APPLE)
  set (requireslib)
endif ()