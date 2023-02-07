INCLUDE(FetchContent)

IF(NOT fmt)
	FetchContent_Declare(fmt
		GIT_REPOSITORY https://github.com/fmtlib/fmt.git
		GIT_TAG "9.1.0") # or whatever tag you want

	FetchContent_GetProperties(fmt)

	FetchContent_Populate(fmt)

	ADD_SUBDIRECTORY(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()

IF(NOT spdlog)
	FetchContent_Declare(spdlog
		GIT_REPOSITORY https://github.com/gabime/spdlog.git
		GIT_TAG "v1.11.0") # or whatever tag you want

	FetchContent_GetProperties(spdlog)

	FetchContent_Populate(spdlog)

	OPTION(SPDLOG_FMT_EXTERNAL ON)

	ADD_SUBDIRECTORY(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()
