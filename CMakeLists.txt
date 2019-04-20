cmake_minimum_required(VERSION 3.13)
project(map C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic-errors -Werror -DNDEBUG")
add_executable(map map.c map.h map_example_test.c cmake-build-debug test_utilities.h )

