INCLUDE(FetchContent)

IF(NOT taskscheduler)
	FetchContent_Declare(taskscheduler
		GIT_REPOSITORY https://github.com/voldien/task-scheduler.git
		GIT_TAG 347e85411b76660528c5d7de1f7453e95a8625bb) # or whatever tag you want

	FetchContent_GetProperties(taskscheduler)

	IF(NOT taskscheduler_POPULATED)
	    FetchContent_Populate(taskscheduler)
	ELSE()
		MESSAGE( WARNING "Could not find taskscheduler source code")
	ENDIF()
	
	IF(taskscheduler_POPULATED)
		ADD_SUBDIRECTORY(${taskscheduler_SOURCE_DIR} ${taskscheduler_BINARY_DIR} EXCLUDE_FROM_ALL)
	ENDIF()
ENDIF()