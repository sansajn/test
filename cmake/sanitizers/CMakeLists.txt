cmake_minimum_required(VERSION 3.16)
project(sanitizer-test CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

find_package(Catch2 2.13.8 REQUIRED)

add_executable(sanitizer sanitizer.cpp)
target_link_libraries(sanitizer PRIVATE Catch2::Catch2WithMain)

# enable address sanitizer for Debug build
if (CMAKE_BUILD_TYPE STREQUAL Debug)
	# TODO: enable also other sanitizers (like undefined) 
	target_compile_options(sanitizer PRIVATE -fsanitize=address)
	target_link_options(sanitizer PUBLIC -fsanitize=address)
endif()
