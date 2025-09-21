cmake_minimum_required(VERSION 3.20)

# Basic smoke test: app prints usage on --help and exits 0
add_test(NAME app_help_works
         COMMAND $<TARGET_FILE:main_app> --help)

include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Unit tests target using project code (CLIOptions)
add_executable(unit_tests
  ${CMAKE_CURRENT_LIST_DIR}/test_clioptions.cpp
  ${CMAKE_SOURCE_DIR}/app/src/CLIOptions.cpp
)
target_include_directories(unit_tests
  PRIVATE
    ${CMAKE_SOURCE_DIR}/app/src
)
target_link_libraries(unit_tests
  PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)


