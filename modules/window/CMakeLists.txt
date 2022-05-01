
ADD_LIBRARY(fragcore-window INTERFACE)


TARGET_INCLUDE_DIRECTORIES (fragcore-window
	INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
# Set predefined macro for version.

ADD_DEFINITIONS(fragcore-window fragcore)