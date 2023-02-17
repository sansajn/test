cmake_minimum_required(VERSION 3.16)
project(mqtt-dump CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

find_package(Boost 1.71 REQUIRED)

add_executable(main main.cpp)
