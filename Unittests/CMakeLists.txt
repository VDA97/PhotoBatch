set(FETCHCONTENT_QUIET OFF CACHE BOOL "Desabilita o modo quiet to FetchContent" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

add_executable(TestArgumentParser "TestArgumentParser.cpp")
target_link_libraries(TestArgumentParser PRIVATE gtest_main)