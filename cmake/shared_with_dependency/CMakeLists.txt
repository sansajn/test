project(use_shared)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -O0 -g")
add_library(a SHARED a.cpp)
add_library(b SHARED b.cpp)
add_executable(useshared main.cpp)
target_link_libraries(useshared a b)
