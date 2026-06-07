cmake_minimum_required(VERSION 3.20)
project(pso VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags for performance
add_compile_options(-O3 -march=native -Wall -Wextra)

# Header-only core library
add_library(pso INTERFACE)
target_include_directories(pso INTERFACE include/)

# Benchmarks
add_executable(benchmark benchmarks/benchmark.cpp)
target_link_libraries(benchmark PRIVATE pso)

# Examples
add_executable(basic examples/basic.cpp)
target_link_libraries(basic PRIVATE pso)

# Tests
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests/test_pso.cpp)
target_link_libraries(tests PRIVATE pso GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)
