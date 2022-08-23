cmake_minimum_required(VERSION 3.16)
project(sanitizer-test CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

find_package(Catch2 2.13.8 REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE Catch2::Catch2WithMain)

include(ClangTidy)
AddClangTidy(main)
