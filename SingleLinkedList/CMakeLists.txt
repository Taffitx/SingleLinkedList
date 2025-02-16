cmake_minimum_required(VERSION 3.11)

# Set the project name
project(SingleLinkedListProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(SingleLinkedList src/main.cpp)

# Specify the include directory
target_include_directories(SingleLinkedList PRIVATE include)
