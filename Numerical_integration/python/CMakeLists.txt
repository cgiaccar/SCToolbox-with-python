cmake_minimum_required(VERSION 3.27.4)
project(approximator)

# Setting Local Variables
set(PROJECT_INCLUDE_DIR "../include")
set(PROJECT_SOURCE_DIR "../src")

set(HEADER_FILES
    ${PROJECT_INCLUDE_DIR}/approximator.hpp
    ${PROJECT_INCLUDE_DIR}/midpoint_approximator.hpp
    ${PROJECT_INCLUDE_DIR}/simpson_approximator.hpp
    ${PROJECT_INCLUDE_DIR}/trapezoidal_approximator.hpp
)
set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/approximator.cpp
    ${PROJECT_SOURCE_DIR}/midpoint_approximator.cpp
    ${PROJECT_SOURCE_DIR}/simpson_approximator.cpp
    ${PROJECT_SOURCE_DIR}/trapezoidal_approximator.cpp
)

#set(MUPARSERX_ROOT "/usr/local")

# pybind11 linking
find_package(pybind11 REQUIRED)
include_directories(SYSTEM ${pybind11_INCLUDE_DIRS})
pybind11_add_module(approximator ${SOURCE_FILES} approximator_py.cpp)

# muParserX linking
find_package(muparserx REQUIRED)
target_include_directories(approximator PRIVATE ${MUPARSERX_INCLUDE_DIR}/../)
target_link_libraries(approximator PRIVATE ${MUPARSERX_LIBRARY})
#find_library(MUPARSERX muParserX REQUIRED)
#target_link_libraries(approximator PRIVATE ${MUPARSERX})