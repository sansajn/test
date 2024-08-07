cmake_minimum_required(VERSION 3.16)
project(conan-sample CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

enable_testing()

# following lines handles library dependencies
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(hello hello.cpp)
target_link_libraries(hello PRIVATE ${CONAN_LIBS})
