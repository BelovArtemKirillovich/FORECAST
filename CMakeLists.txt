cmake_minimum_required(VERSION 3.15)
project(Lab2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(BUILD_COVERAGE "Build with code coverage reporting" OFF)


add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(tests)
