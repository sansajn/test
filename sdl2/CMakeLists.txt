cmake_minimum_required(VERSION 3.16)
project(sdl2-sample CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

find_package(SDL2 2.0 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

add_executable(triangle_anim triangle_anim.cpp)
target_link_libraries(triangle_anim PRIVATE SDL2::SDL2)