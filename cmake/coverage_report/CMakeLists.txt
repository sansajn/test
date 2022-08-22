cmake_minimum_required(VERSION 3.16)
project(catch2-test CXX)

enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

find_package(Catch2 2.13.8 REQUIRED)

add_executable(covtest covtest.cpp)
target_link_libraries(covtest PRIVATE Catch2::Catch2WithMain)

# enable coverage support for Debug build
if (CMAKE_BUILD_TYPE STREQUAL Debug)
	target_compile_options(covtest PRIVATE --coverage)
	target_link_options(covtest PUBLIC --coverage)
	add_custom_command(TARGET covtest PRE_BUILD COMMAND
		find ${CMAKE_BINARY_DIR} -type f 
		-name '*.gcda' -exec rm {} +)
endif()

# generate coverage report
include(Coverage)
AddCoverage(covtest)

# automatic test registration for hello target
include(Catch)
catch_discover_tests(covtest)
