cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)
add_executable(hello-world hello-world.cpp)
add_library(message
    STATIC
        Message.hpp
        Message.cpp)
target_link_libraries(hello-world message)