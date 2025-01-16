##
## EPITECH PROJECT, 2022
## CMakeLists.txt
## File description:
## CMake build script for core project
##

cmake_minimum_required(VERSION 3.10)
project(core)

# Set C++23 standard for the project
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Condition pour macOS
#if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Spécifier le compilateur GCC version 13 installé via Homebrew sur macOS
#    set(CMAKE_CXX_COMPILER "/opt/homebrew/Cellar/gcc/13.2.0/bin/g++-13")
#endif()

# Define the source directory and collect all CPP files
set(SOURCES
        src/ALight.cpp
        src/ALight.hpp
        src/AMaterial.cpp
        src/AMaterial.hpp
        src/AmbientLight.hpp
        src/ANode.cpp
        src/ANode.hpp
        src/Camera.hpp
        src/Color.hpp
        src/Cone.hpp
        src/Cylinder.hpp
        src/Dielectric.hpp
        src/DirectionalLight.hpp
        src/DLLoader.hpp
        src/EventCallbackWrapper.hpp
        src/FlatColor.hpp
        src/GeometryBase.hpp
        src/Group.hpp
        src/ILight.hpp
        src/IMaterial.hpp
        src/INode.hpp
        src/Intersection.hpp
        src/Interval.hpp
        src/Lambertian.hpp
        src/LightFactory.hpp
        src/Light.hpp
        src/LimitedCone.hpp
        src/LimitedCylinder.hpp
        src/main.cpp
        src/MaterialFactory.hpp
        src/Material.hpp
        src/MathUtils.hpp
        src/Matrix.cpp
        src/Matrix.hpp
        src/Metal.hpp
        src/Parser.cpp
        src/Parser.hpp
        src/PhongReflectionModel.hpp
        src/Plane.hpp
        src/Point.cpp
        src/Point.hpp
        src/PointLight.hpp
        src/Ray.hpp
        src/Rectangle.hpp
        src/Renderer.hpp
        src/SafeBuffer.hpp
        src/SafeCounter.hpp
        src/ShapeFactory.cpp
        src/ShapeFactory.hpp
        src/Shape.hpp
        src/Sphere.hpp
        src/TaskQueue.hpp
        src/TransformationFactory.hpp
        src/Transformation.hpp
        src/Triangle.hpp
        src/Vector.cpp
        src/Vector.hpp
        src/Quad.hpp

)

# Find SFML components
find_package(SFML COMPONENTS graphics audio system REQUIRED)
if(SFML_FOUND)
    message(STATUS "SFML found, include directories: ${SFML_INCLUDE_DIRS}")
else()
    message(WARNING "SFML not found")
endif()

# Create the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Define common compile options
set(COMMON_COMPILE_OPTIONS -ftree-vectorize -march=native -mtune=native)

# Adjust compile options based on the platform
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND COMMON_COMPILE_OPTIONS -fopenmp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND COMMON_COMPILE_OPTIONS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND COMMON_COMPILE_OPTIONS /MP)
endif()

# Options de compilation pour Debug et Release
set(DEBUG_OPTIONS ${COMMON_COMPILE_OPTIONS} -std=c++17 -O3 -g)
set(RELEASE_OPTIONS ${COMMON_COMPILE_OPTIONS} -std=c++17 -O3 -DNDEBUG)

# Apply compiler options
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
        $<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>
        $<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>
)

# Link SFML libraries
target_link_libraries(${CMAKE_PROJECT_NAME} sfml-graphics sfml-audio sfml-system dl)

include_directories(${GUI_INTERFACE_DIR})

# Find TBB library
find_library(TBB_LIB tbb)

# Check if the TBB library was found
if(NOT TBB_LIB)
    message(FATAL_ERROR "TBB library not found")
endif()

# Link TBB library
target_link_libraries(${CMAKE_PROJECT_NAME} ${TBB_LIB})

# Find libconfig++ library
find_library(LIBCONFIGPP config++)

if (NOT LIBCONFIGPP)
    message(FATAL_ERROR "libconfig++ library not found")
endif()

# Lier libconfig++ avec votre exécutable
target_link_libraries(${CMAKE_PROJECT_NAME} ${LIBCONFIGPP})
