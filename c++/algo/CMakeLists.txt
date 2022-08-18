# Use `cmake -B BUILD_TREE -S SOURCE_TREE` to generate makefile 
# and `cmake --build BUILD_TREE`, in case only one target is required 
# use `cmake --build BUILD_TREE --target TARGET` command.
cmake_minimum_required(VERSION 3.16)
project(c++-algo CXX)

# per project defaults
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

add_executable(all_any_none_of all_any_none_of.cpp)
add_executable(for_each for_each.cpp)
add_executable(count count.cpp)
add_executable(binary_search binary_search.cpp)
add_executable(nth_element nth_element.cpp)
add_executable(partition partition.cpp)
add_executable(remove_if_on_set remove_if_on_set.cpp)
add_executable(remove_if remove_if.cpp)
add_executable(repeated_2_times repeated_2_times.cpp)
add_executable(reverse_only_letters reverse_only_letters.cpp)
add_executable(reverse reverse.cpp)
add_executable(serch_rotated search_rotated.cpp)
add_executable(sorted_squares sorted_squares.cpp)
add_executable(transform_count_lines transform_count_lines.cpp)

# TODO: do not compile sample in case TBB not installed with a message
find_package(TBB 2020.1 REQUIRED)
add_executable(parallel_transform parallel_transform.cpp)
target_link_libraries(parallel_transform TBB::tbb)
