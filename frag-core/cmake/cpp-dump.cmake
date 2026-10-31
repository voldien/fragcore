INCLUDE(FetchContent)

FetchContent_Declare(cpp_dump_source
	GIT_REPOSITORY https://github.com/philip82148/cpp-dump.git
	GIT_TAG v0.7.0
	EXCLUDE_FROM_ALL
) # or whatever tag you want

FetchContent_MakeAvailable(cpp_dump_source)