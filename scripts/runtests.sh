cmake . . -DCMAKE_BUILD_TYPE=Debug
make
./tests/tests_main

lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_report