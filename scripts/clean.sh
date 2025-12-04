#!/bin/bash
set -e # exit if any command fails

# Remove all contents of the build directory
if [ -d build ]; then
    echo "Cleaning build directory..."
    rm -rf build/*
    echo "Build directory cleaned."
else
    echo "Build directory does not exist."
fi
