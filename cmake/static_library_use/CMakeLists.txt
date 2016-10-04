cmake_minimum_required(VERSION 2.8)

project(main)

add_library(version version.cpp)

include_directories(.)

add_executable(main main.cpp)
target_link_libraries(main version)
