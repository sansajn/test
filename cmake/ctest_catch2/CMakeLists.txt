cmake_minimum_required(VERSION 3.16)
project(catch2-test CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

find_package(Catch2 2.13.8 REQUIRED)

add_executable(hello hello.cpp)
target_link_libraries(hello PRIVATE Catch2::Catch2WithMain)

# automatic test registration for hello target
include(Catch)
catch_discover_tests(hello)
