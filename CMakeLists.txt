##
## EPITECH PROJECT, 2024
## CMakeLists.txt
## File description:
## CMake configuration for the raytracer project
##

cmake_minimum_required(VERSION 3.10)
project(raytracer CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/plugins)

add_subdirectory(plugins_src)
add_subdirectory(core)

find_library(CRITERION_LIB criterion)

if(CRITERION_LIB)
    add_subdirectory(tests)
else()
    message(WARNING "Lib Criterion not found.")
endif()
