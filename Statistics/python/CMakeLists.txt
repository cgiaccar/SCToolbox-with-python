cmake_minimum_required(VERSION 3.27.4)
project(statistics)

# Setting Local Variables
set(PROJECT_INCLUDE_DIR "../include")
set(PROJECT_SOURCE_DIR "../src")

set(HEADER_FILES
    ${PROJECT_INCLUDE_DIR}/fifo_map.hpp
    ${PROJECT_INCLUDE_DIR}/statistics.hpp
)
set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/statistics.cpp
)

# pybind11 linking
find_package(pybind11 REQUIRED)
include_directories(SYSTEM ${pybind11_INCLUDE_DIRS})
pybind11_add_module(statistics ${SOURCE_FILES} statistics_py.cpp)

target_compile_features(statistics PRIVATE cxx_std_17)
target_compile_options(statistics PRIVATE -Wall -Wpedantic)

# Boost linking
find_package(Boost 1.83.0 REQUIRED)
link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(statistics PRIVATE ${Boost_LIBRARIES})