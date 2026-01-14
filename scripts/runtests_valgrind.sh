cmake . . -DCMAKE_BUILD_TYPE=Debug
make
valgrind --leak-check=full --show-leak-kinds=all  ./tests/tests_main
