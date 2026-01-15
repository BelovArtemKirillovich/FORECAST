add_executable(run_tests tests.cpp)

target_link_libraries(run_tests PRIVATE weather_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)