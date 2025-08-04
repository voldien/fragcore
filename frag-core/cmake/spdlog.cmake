INCLUDE(FetchContent)

IF(NOT fmt)
	FetchContent_Declare(fmt
		GIT_REPOSITORY https://github.com/fmtlib/fmt.git
		GIT_TAG "11.2.0")

	FetchContent_GetProperties(fmt)

	IF(NOT fmt_POPULATED)
		FetchContent_Populate(fmt)
	ELSE()
		MESSAGE( WARNING "Could not find fmt source code")
	ENDIF()


	IF(fmt_POPULATED)
		ADD_SUBDIRECTORY(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR} EXCLUDE_FROM_ALL)
	ENDIF()
ENDIF()

IF(NOT spdlog)
	FetchContent_Declare(spdlog
		GIT_REPOSITORY https://github.com/gabime/spdlog.git
		GIT_TAG "v1.15.3")

	FetchContent_GetProperties(spdlog)

	IF(NOT spdlog_POPULATED)
		FetchContent_Populate(spdlog)
	ELSE()
		MESSAGE( WARNING "Could not find spdlog source code")
	ENDIF()


	SET(SPDLOG_FMT_EXTERNAL ON)
	IF(spdlog_POPULATED)
		ADD_SUBDIRECTORY(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR} EXCLUDE_FROM_ALL)
	ENDIF()
ENDIF()