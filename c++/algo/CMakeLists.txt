# Use `cmake -B BUILD_TREE -S SOURCE_TREE` to generate makefile 
# and `cmake --build BUILD_TREE`, in case only one target is required 
# use `cmake --build BUILD_TREE --target TARGET` command.
cmake_minimum_required(VERSION 3.16)
project(c++-algo)

# per project defaults
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(for_each for_each.cpp)
add_executable(count count.cpp)
add_executable(binary_search binary_search.cpp)
