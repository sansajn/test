cmake_minimum_required(VERSION 3.10)
project(HelloGMock)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try to find Google Test and GMock
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(GTEST gtest)
    pkg_check_modules(GTEST_MAIN gtest_main)
    pkg_check_modules(GMOCK gmock)
    pkg_check_modules(GMOCK_MAIN gmock_main)
endif()

# If pkg-config didn't find it, try standard find_package
if(NOT GTEST_FOUND)
    find_package(GTest QUIET)
endif()

# Create both executables
add_executable(hello hello.cpp)
add_executable(real real.cpp)

# Link both executables with Google Test and GMock
set(TARGETS hello real)

foreach(target ${TARGETS})
    if(GTest_FOUND)
        target_link_libraries(${target} GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
    elseif(GTEST_FOUND AND GMOCK_FOUND)
        target_include_directories(${target} PRIVATE ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
        target_link_libraries(${target} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${GMOCK_LIBRARIES} ${GMOCK_MAIN_LIBRARIES} pthread)
    else()
        # Manual linking as fallback
        target_link_libraries(${target} gtest gtest_main gmock gmock_main pthread)
    endif()
endforeach()

# Enable testing
enable_testing()

# Add both tests
add_test(NAME HelloTests COMMAND hello)
add_test(NAME RealWorldTests COMMAND real)
