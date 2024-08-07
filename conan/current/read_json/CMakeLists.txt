# Set project properties
cmake_minimum_required(VERSION 3.12)
project(read_json_project LANGUAGES CXX)

# Set directory variables
set(SOURCE_DIR src)
set(INCLUDE_DIR include)

# Access JSON package
find_package(nlohmann_json REQUIRED)

# Define target
add_executable(read_json ${SOURCE_DIR}/main.cpp)

# Set target properties
target_compile_options(read_json PUBLIC -Wall -std=c++17)
target_link_libraries(read_json nlohmann_json::nlohmann_json)
target_include_directories(read_json PUBLIC ${INCLUDE_DIR})