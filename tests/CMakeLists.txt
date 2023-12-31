include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

enable_testing()

add_executable(ftquant_tests
        u_MonteCarlo.cpp
        u_SyntaxParser.cpp
        u_PartialDiffEq.cpp
        i_main.cpp
)

target_link_libraries(ftquant_tests PUBLIC
        GTest::GTest
        ftqlib
)

include(GoogleTest)
message(STATUS "GTest loaded")
gtest_discover_tests(ftquant_tests)
