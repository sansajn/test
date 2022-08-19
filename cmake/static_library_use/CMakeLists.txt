cmake_minimum_required(VERSION 3.16)

project(static-lib)

add_library(version STATIC version.cpp)

include_directories(.)

add_executable(main main.cpp)
target_link_libraries(main version)
