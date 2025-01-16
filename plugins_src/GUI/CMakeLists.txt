## EPITECH PROJECT, 2024
## CMakeLists.txt
## File description:
## CMake configuration for the raytracer project

cmake_minimum_required(VERSION 3.20)
project(raytracer_gui CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O3")

set(INTERFACE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/interface")
set(ABSTRACT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/abstract")

set(GUI_INTERFACE_DIR "${INTERFACE_DIR}" CACHE INTERNAL "GUI interface directory")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

if(SFML_FOUND)
    message(STATUS "SFML found, include directories: ${SFML_INCLUDE_DIRS}")
else()
    message(WARNING "SFML not found")
endif()

add_library(${PROJECT_NAME} SHARED
        src/SFMLGuiPlugin.cpp
        src/SFMLGuiPlugin.hpp
        abstract/AGuiPlugin.cpp
        abstract/AGuiPlugin.hpp
        interface/IGuiPlugin.hpp
        interface/IEvent.hpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".so")

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${INTERFACE_DIR}
        PRIVATE ${ABSTRACT_DIR}
)

message(STATUS "Interface dir: ${INTERFACE_DIR}")
