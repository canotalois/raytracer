#!/bin/bash

# Check if raytracer or build exist and if remove both of them
if [ "$1" == "fclean" ]; then
    if [ -d build ]; then
        rm -rf build
    fi
    if [ -d plugins ]; then
        rm -rf plugins
    fi
    if [ -f raytracer ]; then
        rm raytracer
    fi
    if [ -f unit_tests ]; then
        rm unit_tests
    fi
    exit 0
fi

# Create the build directory if it does not exist
if [ ! -d "build" ]; then
    mkdir build
fi

# Configure the project
cmake -DCMAKE_BUILD_TYPE=Debug -B build -S . # ou -DCMAKE_BUILD_TYPE=Release
if [ $? -ne 0 ]; then
    echo -e "\033[0;31mError during project configuration.\033[0m"
    exit 1
fi

# Build the project
cmake --build build -- -j
if [ $? -ne 0 ]; then
    echo -e "\033[0;31mError during project build.\033[0m"
    exit 1
fi

# Display the build complete message in green
echo -e "\033[0;32mBuild complete.\033[0m"

# ./raytracer config_files/scene_graph.cfg
