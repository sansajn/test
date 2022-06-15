cmake_minimum_required(VERSION 3.16)

project(image VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(image
	main.cpp
)

target_link_libraries(image PRIVATE Qt6::Widgets)
