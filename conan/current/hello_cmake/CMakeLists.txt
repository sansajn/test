# Set the minimum version of CMake
cmake_minimum_required(VERSION 3.12)

# Set the project's name and properties
project(hello_cmake_project LANGUAGES CXX)

# Create a variable named SOURCE_DIR
set(SOURCE_DIR src)

# Create a variable named INCLUDE_DIR
set(INCLUDE_DIR include)

# Define a target executable
add_executable(hello_cmake ${SOURCE_DIR}/main.cpp)

# Set compile options for the target
target_compile_options(hello_cmake PUBLIC -Wall -std=c++17)

# Set the include directories for the target
target_include_directories(hello_cmake PUBLIC ${INCLUDE_DIR})
