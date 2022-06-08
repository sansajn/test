cmake_minimum_required(VERSION 3.16)

project(hello VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(hello
	main.cpp
)

target_link_libraries(hello PRIVATE Qt6::Widgets)
