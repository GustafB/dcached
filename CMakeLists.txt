cmake_minimum_required(VERSION 3.24)

project(dcached VERSION 0.0
                DESCRIPTION "persistent in-memory cache"
                LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
