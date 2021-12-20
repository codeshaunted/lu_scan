# averysumner - lu_scan
# CMakeLists.txt
# root CMake file

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lu_scan VERSION 1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

# internal
add_subdirectory("source")

# third party
add_subdirectory("third_party")