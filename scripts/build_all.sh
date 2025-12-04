#!/bin/bash
set -e # exit immediately if a command fails

# Path to vcpkg toolchain file
VCPKG_TOOLCHAIN_FILE=/home/chrinovic/vcpkg/scripts/buildsystems/vcpkg.cmake

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Run CMake configure
cmake .. -DCMAKE_TOOLCHAIN_FILE=${VCPKG_TOOLCHAIN_FILE}

# Build the project
cmake --build .
