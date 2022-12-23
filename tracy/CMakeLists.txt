cmake_minimum_required(VERSION 3.16)
project(tracy-test CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)

add_executable(main main.cpp)

# tracy support
include(FetchContent)

FetchContent_Declare(external-tracy
	GIT_REPOSITORY https://github.com/wolfpld/tracy.git
	GIT_TAG v0.9
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(external-tracy)

target_link_libraries(main PUBLIC TracyClient)
