#this is set up so you can run on VSC/mac
cmake_minimum_required(VERSION 3.10)

project(test)

# Use modern C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable warnings
add_compile_options(-Wall)

# Set build type
set(CMAKE_BUILD_TYPE Debug)

# Add executable
add_executable(test test_main.cpp)