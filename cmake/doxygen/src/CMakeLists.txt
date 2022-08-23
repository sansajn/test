find_package(Catch2 2.13.8 REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE Catch2::Catch2WithMain)
