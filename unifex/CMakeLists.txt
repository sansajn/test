cmake_minimum_required(VERSION 3.16)
project(unifex-test CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

# we are using unifex version 0.1
find_package(unifex REQUIRED)

add_executable(execute execute.cpp)
target_link_libraries(execute PRIVATE unifex)

add_executable(version version.cpp)
target_link_libraries(version PRIVATE unifex)
