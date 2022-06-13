cmake_minimum_required(VERSION 3.16)

project(input VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(input
	main.cpp
)

target_link_libraries(input PRIVATE Qt6::Widgets)
