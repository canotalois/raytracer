#!/bin/bash

# Create the build directory if it does not exist
if [ ! -d "build" ]; then
    mkdir build
fi

# Configure the project for unit tests
cmake -DCMAKE_BUILD_TYPE=Debug -B build -S .
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

# Run unit tests
./unit_tests
if [ $? -ne 0 ]; then
    echo -e "\033[0;31mError during unit tests execution.\033[0m"
    exit 1
fi

# Display the tests complete message in green
echo -e "\033[0;32mUnit tests complete.\033[0m"
