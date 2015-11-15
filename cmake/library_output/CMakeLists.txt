project(version)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/jniLibs")
add_library(version main.cpp)
