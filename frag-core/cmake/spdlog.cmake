

INCLUDE(FetchContent)

FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
		GIT_TAG "v1.8.5") # or whatever tag you want

FetchContent_GetProperties(spdlog)

IF(NOT spdlog)
	FetchContent_Populate(spdlog)
	ADD_SUBDIRECTORY(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()


