# dependencies: libgdal-dev (3.4.1)
cmake_minimum_required(VERSION 3.16)
project(gdal-test CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GDAL CONFIG)  # we have 3.4.1 version installed
if(NOT GDAL_FOUND)
    find_package(GDAL 3.4.1 REQUIRED)
endif()

add_executable(geo_transform geo_transform.cpp)
target_link_libraries(geo_transform PRIVATE GDAL::GDAL)

add_executable(projection_ref projection_ref.cpp)
target_link_libraries(projection_ref PRIVATE GDAL::GDAL)

add_executable(geotiff_info geotiff_info.cpp)
target_link_libraries(geotiff_info PRIVATE GDAL::GDAL)