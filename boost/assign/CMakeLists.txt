cmake_minimum_required(VERSION 3.16)
project(test-assign CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

find_package(Boost 1.71 REQUIRED)

add_executable(fill_vector fill_vector.cpp)
add_executable(insert_map insert_map.cpp)
