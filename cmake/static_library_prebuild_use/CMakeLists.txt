cmake_minimum_required(VERSION 2.8)

project(main)

include_directories(.)

add_executable(main main.cpp)
target_link_libraries(main ${CMAKE_SOURCE_DIR}/libversion.a)
