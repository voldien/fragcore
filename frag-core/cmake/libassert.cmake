INCLUDE(FetchContent)

FetchContent_Declare(libassert_source
    GIT_REPOSITORY https://github.com/jeremy-rifkin/libassert
    GIT_TAG v2.2.1
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(libassert_source)

