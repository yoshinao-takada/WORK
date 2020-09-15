cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(base LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    /usr/include/opencv4
)
add_compile_options(
    -Wno-unknown-pragmas
    -DDATA_DIR="${CMAKE_CURRENT_LIST_DIR}/UTData/"
)
add_subdirectory(base_l)
add_subdirectory(base_g)
add_subdirectory(base_o)
add_subdirectory(base_z)
add_subdirectory(UTbase_l)
add_subdirectory(UTbase_g)
add_subdirectory(UTbase_o)

