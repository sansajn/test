cmake_minimum_required(VERSION 3.16)
project(catch2-test CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

find_package(Catch2 3.4.0 REQUIRED)

add_executable(capture capture.cpp)
target_link_libraries(capture PRIVATE Catch2::Catch2WithMain)

# automatic test registration sample (for hello target)
#include(Catch)
#catch_discover_tests(hello)
