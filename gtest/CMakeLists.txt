cmake_minimum_required(VERSION 3.16)
project(gtest-test CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

# load Google test library
find_package(GTest 1.11.0 REQUIRED)

add_executable(basic_test basic_test.cpp)
target_link_libraries(basic_test PRIVATE GTest::gtest)

add_executable(fixture_test fixture_test.cpp)
target_link_libraries(fixture_test PRIVATE GTest::gtest)

add_executable(parametrized_test parametrized_test.cpp)
target_link_libraries(parametrized_test PRIVATE GTest::gtest)

# automatic test registration for hello target
include(GoogleTest)
gtest_discover_tests(basic_test)
gtest_discover_tests(fixture_test)
