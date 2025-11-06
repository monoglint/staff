cmake_minimum_required(VERSION 3.10)
project(staff VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Your targets here...

add_executable(staff
    src/main.cc
    src/internal.cc
    src/api.cc
)

# Add include directory
target_include_directories(staff PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Set C++ standard
set_property(TARGET staff PROPERTY CXX_STANDARD 17)