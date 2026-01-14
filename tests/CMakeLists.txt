# # Поиск GoogleTest через find_package # требуется запуск scripts/install.sh
# find_package(GTest REQUIRED)

# # Тестовый исполняемый файл
# add_executable(tests_main src/main.cpp)
# target_link_libraries(tests_main weather_lib GTest::gtest GTest::gtest_main)

# # Настройки покрытия
# if(BUILD_COVERAGE)
#     target_compile_options(tests_main PRIVATE --coverage -fprofile-arcs -ftest-coverage)
#     target_link_libraries(tests_main PRIVATE --coverage gcov)
# endif()

# include(GoogleTest)
# gtest_discover_tests(tests_main)